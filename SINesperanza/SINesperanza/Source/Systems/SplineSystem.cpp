#include "Systems/SplineSystem.h"
#include "Graphics/Renderer.h"
#include "Core/WorldManager.h"

namespace SIN
{
	SplineSystem::SplineSystem()
	{
		Register();
		frameOne = true;
	}


	void SplineSystem::OnUpdate(float a_DeltaTime)
	{
		Renderer* renderer = WorldManager::GetSingleton().GetRenderer();

		auto view = GetEntityRegister().view<SplineComponent>();

		for (auto entity : view)
		{
			SplineComponent& spline = GetEntityRegister().get<SplineComponent>(entity);

			if (spline.GetPoints().empty()) return;
			
			// Draw points.
			for (size_t i = 0; i < spline.GetPoints().size(); i++)
			{
				const float x = spline.GetPoints()[i].x;
				const float y = spline.GetPoints()[i].y;
				renderer->MakePoint(x, y, 5);
			}

			if (spline.GetPoints().size() < 2) return;
			
			if (spline.GetType() == SplineType::Bezier)
			{
				const int resolution = 5 * spline.GetPoints().size();
				const float step = 1.f / resolution;

				for (size_t i = 0; i < resolution; i++)
				{
					const float t = i * step;
					const glm::vec2 point1 = spline.GetPoint(i * step);
					const glm::vec2 point2 = spline.GetPoint((i + 1) * step);

					renderer->MakeLine(point1.x, point1.y, point2.x, point2.y);
				}

				for (size_t i = 0; i < spline.GetPoints().size() - 1; i++)
				{
					const int resolutionStriped = glm::distance(spline.GetPoints()[i], spline.GetPoints()[i + 1]) * 2;
					
					for (size_t k = 0; k < resolutionStriped; k++)
					{
						const glm::vec2 point = glm::lerp(spline.GetPoints()[i], spline.GetPoints()[i + 1], (float)(k) / resolutionStriped);
						renderer->MakePoint(point.x, point.y, 1);
					}
				}
			}
			else if (spline.GetType() == SplineType::CubicBezier)
			{
				const int resolution = 5 * spline.GetPoints().size();
				const float step = 1.f / resolution;
				
				for (size_t i = 0; i < resolution; i++)
				{
					const float t = i * step;
					const glm::vec2 point1 = spline.GetPoint(i * step);
					const glm::vec2 point2 = spline.GetPoint((i + 1) * step);

					renderer->MakeLine(point1.x, point1.y, point2.x, point2.y);
				}

				int size = static_cast<int>(spline.GetPoints().size() / 3.f);
				
				for (size_t i = 0; i < size; i++)
				{
					int start = i * 3;

					for (size_t k = 1; k < 11; k++)
					{
						glm::vec2 lerp = glm::lerp(spline.GetPoints()[start], spline.GetPoints()[start + 1], k * 0.1f);
						renderer->MakePoint(lerp.x, lerp.y, 1);
						lerp = glm::lerp(spline.GetPoints()[start + 2], spline.GetPoints()[start + 3], k * 0.1f);
						renderer->MakePoint(lerp.x, lerp.y, 1);
					}
				}
			}
			else
			{
				// Draw lines.
				for (size_t i = 0; i < spline.GetPoints().size() - 1; i++)
				{
					const glm::vec2 point1 = spline.GetPoints()[i];
					const glm::vec2 point2 = spline.GetPoints()[i + 1];
					renderer->MakeLine(point1.x, point1.y, point2.x, point2.y);
				}
			}
		}
	}
}


