#pragma once
#include "Core/Systems.h"
#include "Components/AIComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicsComponent.h"

namespace SIN
{	
	class AISystem :
		public Systems
	{
	public:
		AISystem();
		~AISystem(){}
		void Initialize(){}
		void OnUpdate(float a_Deltatime);
		
	private:
		
		void DoBehaviour(AIComponent* ai_component, float a_Deltatime, Tire* a_Tire);
		void UpdateDrift(PhysicsComponent* a_physics_component, AIComponent* a_ai_component);
		void HandleDrift(PhysicsComponent* a_physics_component, AIComponent* a_ai_component);
		void KillDrift(PhysicsComponent* a_physics_component, AIComponent* a_ai_component);
	};
}


