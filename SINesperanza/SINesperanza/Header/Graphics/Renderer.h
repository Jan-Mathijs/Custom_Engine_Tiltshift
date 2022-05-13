#pragma once
#include "Core/Systems.h"

namespace SIN {

	/// <summary>
	/// Interface for the graphics API. Implemented in subclasses such as GLRenderer.
	/// Contains requirements for rendering lines (to prevent duplicate code).
	/// Should be created once per application; I'll probably convert it to a singleton.
	/// Inheritance from Systems is to gain access to the entity register when rendering.
	/// </summary>

	class Renderer : public Systems
	{
	public:
		struct Point
		{
			float X, Y, Size, padding = 0.f;
			Point(float x, float y, float size) :
				X(x), Y(y), Size(size)
			{}

			Point() : X(0.f), Y(0.f), Size(1.f) {}
			~Point() {}
		};

		struct Line
		{
			float X0, Y0, X1, Y1;
			Line(float xFrom, float yFrom, float xTo, float yTo) :
				X0(xFrom), Y0(yFrom), X1(xTo), Y1(yTo)
			{}

			Line() : X0(0.f), Y0(0.f), X1(0.f), Y1(0.f) {}
			~Line() {}
		};

		struct Vertex
		{
			float X, Y;
			float TexX, TexY;
			Vertex(float x, float y, float texX, float texY) :
				X(x), Y(y), TexX(texX), TexY(texY)
			{}

			Vertex() : X(0.f), Y(0.f), TexX(0.f), TexY(0.f) {}
			~Vertex() {}
		};

		typedef uint16_t IndexSize; 

		Renderer();
		virtual ~Renderer();
		virtual void OnUpdate(float) {} //Inherited from Systems.h
		virtual void Render(int a_CameraIndex) = 0;

		//Returns true if a line could be created; false if it could not (if the buffer with points is full, max is 128 points)
		bool MakePoint(float x, float y, float size); //Can also be used to state your case
		//Same story
		bool MakeLine(float x0, float y0, float x1, float y1);
	protected:
		static constexpr int g_MaxPointsLines = 12000; //This number is arbitrary 256

		Point* m_Points;
		int m_NumPoints;

		Line* m_Lines;
		int m_NumLines;
	};
}
