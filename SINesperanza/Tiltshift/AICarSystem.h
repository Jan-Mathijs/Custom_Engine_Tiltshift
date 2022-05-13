#pragma once
#include "Core/Systems.h"
#include "Components/TireComponent.h"
#include "Components/AIComponent.h"
class AICarSystem :public SIN::Systems
{
public:
	AICarSystem();
	~AICarSystem();

	void CreateCar();
	virtual void OnUpdate(float a_DeltaTime);
	void SetPhysics(SIN::AIComponent* aicomp, PhysicsComponent* physics_component, entt::entity* Tire1, entt::entity* Tire2, entt::entity* Tire3, entt::entity* Tire4);
	
private:
	b2RevoluteJoint* flJoint;
	b2RevoluteJoint* frJoint;

	int m_AICount = 0;
};