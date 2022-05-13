#pragma once
#include "Core/Systems.h"
#include "Components/TireComponent.h"

namespace SIN
{
	class TireSystem : public Systems
	{
	public:
		TireSystem();
		~TireSystem();
		void Initialize(){}
		void OnUpdate(float a_Deltatime);
	private:
		b2Vec2 getLateralVelocityB2Vec(Tire* a_tire_component);
		void UpdateFriction(Tire* a_tire_component);
		void UpdateDrive(Tire* a_tire_component);
		void UpdateTurn(Tire* a_tire_component);

		b2Vec2 GetForwardVelocity(Tire* a_tire_component);
		
	};
}
