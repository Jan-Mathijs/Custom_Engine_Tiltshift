#pragma once
#include "Components/SplineComponent.h"
#include "Core/Systems.h"

namespace SIN
{	
	class SplineSystem : public Systems
	{

	private:

		bool frameOne;
		entt::entity m_Entity[2];
	
	public:
		
		SplineSystem();
		~SplineSystem() = default;

		void OnUpdate(float a_DeltaTime) override;
	};
}

