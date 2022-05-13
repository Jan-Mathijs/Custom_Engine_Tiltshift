#pragma once

#include "Core/Systems.h"
#include "Components/TireComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"

class PlayerCarSystem :public SIN::Systems
{
public:
	PlayerCarSystem();
	~PlayerCarSystem();

	void CreateCar();
	virtual void OnUpdate(float a_DeltaTime);
private:
	void RegisterInput();
	std::vector<Tire*> m_Tires;
	std::vector<b2Body*> m_tireBodies;
	b2RevoluteJoint* m_FlJoint;
	b2RevoluteJoint* m_FrJoint;

	void HandleDrift(PhysicsComponent* a_physics_component);
	void UpdateDrift(PhysicsComponent* a_physics_component);
	void KillDrift(PhysicsComponent* a_physics_component);
	glm::vec2 m_ForwardSpeed = glm::vec2(0);
	glm::vec2 m_LateralSpeed = glm::vec2(0);

	glm::vec2 m_CurrentCameraPos = glm::vec2(0);
	float m_CurrentCameraRotation = 0.0f;
	float m_CameraLerpScale = 0.4f;
	float m_CameraLerpPositionScale = 4.5f;
	float m_PrevScaler = 0;

	void UpdateCamera(PhysicsComponent* a_physics_component, SIN::TransformComponent* a_transform_component, float m_Deltatime);
	
};