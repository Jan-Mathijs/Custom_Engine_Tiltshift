#pragma once
#include <iostream>
#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/compatibility.hpp"
#include <fstream>
#include "cereal/archives/binary.hpp"

namespace SIN
{
	struct SplineCorner
	{
		glm::vec2 segmentPosition, pre1, pre2, post1, post2;
		int segment;
	};

	enum class SplineType
	{
		Linear,
		QuadraticBezier,
		CubicBezier,
		Bezier
	};
	
	struct SplineComponent
	{
	private:

		std::vector<glm::vec2> m_Points;
		SplineType m_Type;
		float m_Length;

	public:

		SplineComponent()
		{
			m_Points = std::vector<glm::vec2>(0);
			m_Points.emplace_back(glm::vec2(0, 0));
			m_Length = 0;
			m_Type = SplineType::Linear;
		}

		~SplineComponent() = default;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_Type, m_Points, m_Points.size()); // serialize things by passing them to the archive
		}

		template<class Archive>
		void Save(const std::string& a_FileName)
		{
			std::ofstream os(a_FileName.c_str(), std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			for (size_t i = 0; i < m_Points.size(); i++)
			{
				archive(m_Type, m_Points[i], m_Points.size());
			}
		}

		template<class Archive>
		void Load(const std::string& a_FileName)
		{
			std::ofstream os(a_FileName.c_str(), std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			size_t count = 0;

			archive(m_Type, m_Points[0], count);
			m_Points.resize(count);
			
			for (size_t i = 0; i < count; i++)
			{
				archive(m_Type, m_Points.at(i), count);
			}
		}
		
		// ---------------------------------------- \\
		// -------------| Get / Set |-------------- \\
		// ---------------------------------------- \\


		void SetType(const SplineType a_Type) { m_Type = a_Type; }
		[[nodiscard]] float GetLength() const { return m_Length; }
		[[nodiscard]] SplineType GetType() const { return  m_Type; }
		[[nodiscard]] const std::vector<glm::vec2>& GetPoints() const { return m_Points; }

		// ---------------------------------------- \\
		// --------------| Methods |--------------- \\
		// ---------------------------------------- \\

		/// <summary>
		/// Get the pure points of the spline to save them to the harddisk.
		/// </summary>
		std::vector<glm::vec2> GetPureTrackPoints()
		{
			return m_Points;
		}
		

		/// <summary>
		/// Add a new point at location, adds an additional control point when the spline is of type bezier.
		/// </summary>
		void AddPoint(const glm::vec2 a_Point)
		{
			if (m_Type == SplineType::Bezier)
			{
				m_Points.push_back(glm::lerp(m_Points[m_Points.size() - 1], a_Point, 0.5f) + glm::vec2(0, 1));
			}
			else if (m_Type == SplineType::CubicBezier)
			{
				const int index = m_Points.size() - 1;
				m_Points.push_back(glm::lerp(m_Points[index], a_Point, 0.33f));
				m_Points.push_back(glm::lerp(m_Points[index], a_Point, 0.66f));
			}

			m_Points.push_back(a_Point);
			CalculateLength();
		}

		/// <summary>
		/// Returns the number of segments, control points excluded.
		/// </summary>
		[[nodiscard]] size_t GetSize() const
		{
			switch (m_Type)
			{
				case SplineType::Linear:
					return  m_Points.size() - 1;
					
				case SplineType::QuadraticBezier: 
					return m_Points.size() / 2;
				
				case SplineType::CubicBezier: 
					return m_Points.size() / 3;
				
				case SplineType::Bezier: 
					return m_Points.size() / 2;
			}

			return  0;
		}
		
		/// <summary>
		/// Remove point at index, also removes the control point when using bezier spline.
		/// </summary>
		void RemovePoint(const int a_Index)
		{
			if (m_Type == SplineType::Bezier)
			{
				m_Points.erase(m_Points.begin() + a_Index);
			}

			m_Points.erase(m_Points.begin() + a_Index);
			CalculateLength();
		}

		/// <summary>
		/// Move either a segment or control point.
		/// </summary>
		void MovePoint(int a_Index, glm::vec2 a_Position)
		{
			if (a_Index >= 0 && a_Index < m_Points.size())
			{
				m_Points[a_Index] = a_Position;
			}
		}


		// ---------------------------------------- \\
		// ------------| Get Methods |------------- \\
		// ---------------------------------------- \\


		/// <summary>
		/// Retrieves a position on the spline by value t, supports all spline types.
		/// </summary>
		/// <param name="a_t">Range 0 - 1.</param>
		/// <returns>Location on spline.</returns>
		glm::vec2 GetPoint(const float a_t)
		{
			float t = glm::clamp(a_t, 0.f, 1.f);

			if (m_Type == SplineType::Bezier)
			{
				return GetPointBezier(t);
			}
			else if (m_Type == SplineType::CubicBezier)
			{
				return  GetPointCubicBezier(t);
			}
			else
			{
				return GetPointLinear(t);
			}
		}

		/// <summary>
		/// Returns an array of positions with the size specified with count between the specified point and the next one.
		/// </summary>
		glm::vec2* GetSplineInPoints(const int a_Resolution)
		{
			glm::vec2* points = new glm::vec2[a_Resolution];
			const float step = 1.f / a_Resolution;

			for (size_t i = 0; i <= a_Resolution; i++)
			{
				points[i] = GetPoint(i * step);
			}

			return points;
		}

		/// <summary>
		/// Returns an array of positions with the size specified with count between the specified point and the next one.
		/// </summary>
		void GetSplineInPoints(glm::vec2* a_Points, const int a_Resolution)
		{
			const float step = 1.f / a_Resolution;

			for (size_t i = 0; i <= a_Resolution; i++)
			{
				a_Points[i] = GetPoint(i * step);
			}
		}

		/// <summary>
		/// Retrieves a position on the spline by value distance, supports all spline types.
		/// </summary>
		/// <param name="a_Distance"></param>
		/// <returns></returns>
		glm::vec2 GetPointByDistance(const float a_Distance)
		{
			float t = Remap(a_Distance, 0, m_Length, 0, 1);

			return GetPoint(t);
		}

		int GetNextSegmentIndex(const float a_t) const
		{
			float strife;
			
			switch (m_Type)
			{
			case SplineType::Linear:

				return  a_t * GetSize() + 1;

			case SplineType::QuadraticBezier:

				strife = static_cast<int>((a_t * 0.9999f) * GetSize());
				return (strife < 1 ? 1 : strife) * 2;

			case SplineType::CubicBezier:

				strife = static_cast<int>((a_t * 0.9999f) * GetSize());
				return (strife < 1 ? 1 : strife) * 3;

			case SplineType::Bezier:

				strife = static_cast<int>((a_t * 0.9999f) * GetSize());
				return (strife < 1 ? 1 : strife) * 2;
			}

			return  0;
		}

		glm::vec2 GetNextSegmentVec2(const float a_t) const
		{
			return GetPoints()[GetNextSegmentIndex(a_t)];
		}

		// ---------------------------------------- \\
		// ------------| Find Methods |------------ \\
		// ---------------------------------------- \\
		

		/// <summary>
		/// Returns 4 points at the next ceiled segment value.
		/// </summary>
		/// <param name="a_offset">0 - 1 scale modifier</param>
		/// <returns></returns>
		SplineCorner FindNextSegmentCornerPoints(const float a_t, const float a_offset = 0.40f)
		{
			const float pointStep = 1.f / GetSegmentCount();
			const int nextPoint = GetNextSegmentIndex(a_t);

			const float t = nextPoint * pointStep;
			const float offset = glm::clamp(a_offset, 0.f, 1.f);

			const SplineCorner corner
			{
				m_Points[nextPoint],
				GetPoint(t - pointStep * (0.25f * offset)),
				GetPoint(t - pointStep * (0.125f * offset)),
				GetPoint(t + pointStep * (0.25f * offset)),
				GetPoint(t + pointStep * (0.125f * offset)),
				nextPoint
			};

			return corner;
		}

		/// <summary>
		/// Return closest segment to position, also includes control points in the bezier spline.
		/// </summary>
		glm::vec2 FindClosestPoint(glm::vec2 a_Position) const
		{
			int closest = 0;
			float previousDistance = 1000000;

			for (size_t i = 0; i < m_Points.size(); i++)
			{
				float distance = glm::distance(a_Position, m_Points[i]);
				if (distance < previousDistance)
				{
					previousDistance = distance;
					closest = i;
				}
			}

			return m_Points[closest];
		}
	
		// Work in progress
		glm::vec2 FindClosestPosition(glm::vec2 a_Position)
		{

		}

		/// <summary>
		/// Interpolates between two points using the quadratic bezier equation.
		/// </summary>
		/// <param name="p1">First point of the bezier curve.</param>
		/// <param name="p2">The control point for the bezier curve.</param>
		/// <param name="p3">Second point of the bezier curve.</param>
		/// <param name="t">interpolation.</param>
		/// <returns></returns>
		glm::vec2 QuadraticBezier(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) const
		{
			return glm::lerp(glm::lerp(p1, p2, t), glm::lerp(p2, p3, t), t);
		}

		
		
	private:

		glm::vec2 GetPointLinear(const float a_t)
		{
			const int size = m_Points.size() - 1;
			const float pointStep = 1.f / size;
			const int startPoint = a_t * size;

			float t = Remap(a_t, startPoint * pointStep, (startPoint + 1) * pointStep, 0, 1);

			int min = 0;
			int max = static_cast<int>(m_Points.size()) - 1;
			return glm::lerp(m_Points[startPoint], m_Points[glm::clamp(startPoint + 1, min, max)], t);
		}
		
		glm::vec2 GetPointBezier(const float a_t)
		{
			// I only want to look at the actual points.
			// Each time we take the next starting point the control point should be taken into consideration to not be selected.

			const int size = m_Points.size() * 0.5f;
			const float pointStep = 1.f / size;
			
			const int startPoint = static_cast<int>((a_t * 0.9999f) * size) * 2;

			float t = Remap(a_t, startPoint * 0.5f * pointStep, (startPoint + 1) * 0.5f * pointStep, 0, 1) * 0.5f;

			return QuadraticBezier(m_Points[startPoint], m_Points[startPoint + 1], m_Points[startPoint + 2], t);
		}

		glm::vec2 GetPointCubicBezier(const float a_t)
		{
			// I only want to look at the actual points.
			// Each time we take the next starting point the control point should be taken into consideration to not be selected.

			const int size = m_Points.size() / 3.f;
			const float pointStep = 1.f / size;

			const int startPoint = static_cast<int>((a_t * 0.999999f) * size) * 3;

			float in1 = startPoint / 3.f * pointStep;
			float in2 = (startPoint + 1) / 3.f * pointStep;
			
			float t = Remap(a_t, in1, in2, 0, 1) / 3.f;

			glm::vec2 q1 = QuadraticBezier(m_Points[startPoint], m_Points[startPoint + 1], m_Points[startPoint + 2], t);
			glm::vec2 q2 = QuadraticBezier(m_Points[startPoint + 1], m_Points[startPoint + 2], m_Points[startPoint + 3], t);

			return glm::lerp(q1, q2, t);
		}

		float Remap(float val, float in1, float in2, float out1, float out2)
		{
			return out1 + (val - in1) * (out2 - out1) / (in2 - in1);
		}

		void CalculateLength()
		{
			m_Length = 0;
			
			if(m_Points.size() > 1)
			{
				for (size_t i = 0; i < m_Points.size() - 1; i++)
				{
					m_Length += glm::distance(m_Points[i], m_Points[i + 1]);
				}
			}
		}

	public:

		/// <summary>
		/// Returns the amount of segments in this spline.
		/// </summary>
		int GetSegmentCount() const
		{
			switch (m_Type)
			{
			case SplineType::Linear:
				return m_Points.size() - 1;
				
			case SplineType::QuadraticBezier:
				return m_Points.size() * 0.5f;
				
			case SplineType::CubicBezier:
				return m_Points.size() / 3.f;
				
			case SplineType::Bezier:
				return m_Points.size() * 0.5f;
			default:
				return 0;
			}
		}
		
		// An example function whose solution is determined using 
		// Bisection Method. The function is x^3 - x^2  + 2 
		float Bisection(const float x) const
		{
			return x * x * x - x - 1;
		}

		// Derivative of the above function which is 3*x^x - 2*x 
		float Derivative(const float x) const
		{
			return 3 * (x * x) - 1;
		}


		// 
		float NewtonRaphson(const float x) const
		{
			
			//float h = Bisection(x) / Derivative(x);

			//double h = func(x) / derivFunc(x);
			//while (abs(h) >= EPSILON)
			//{
			//	h = func(x) / derivFunc(x);

			//	// x(i+1) = x(i) - f(x) / f'(x)   
			//	x = x - h;
			//}


			//return a_X - h;
		}
		
	};
}