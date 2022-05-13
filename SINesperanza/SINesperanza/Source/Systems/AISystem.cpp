#include "Systems/AISystem.h"
#include <iostream>
#include "Core/WorldManager.h"
#include "glm/glm/glm.hpp"
#include "Graphics/Renderer.h"
#include "Core/AINodeManager.h"
#include "Components/Tags.h"

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

using namespace SIN;

AISystem::AISystem()
{
	Register();
}

void AISystem::OnUpdate(float a_deltatime)
{
	Renderer* renderer = WorldManager::GetSingleton().GetRenderer();

	glm::vec2 tirePosition[4]{glm::vec2(0)};
	
	//get all the entities with these components
	for (auto entity : GetEntityRegister().view<AIComponent, TransformComponent, PhysicsComponent, AITag>())
	{
		//get safe the components
 		AIComponent* l_AIComponent = &GetEntityRegister().get<AIComponent>(entity);
		TransformComponent* l_TransformComponent = &GetEntityRegister().get<TransformComponent>(entity);
		PhysicsComponent* l_physics_component = &GetEntityRegister().get<PhysicsComponent>(entity);
		
		if (l_AIComponent->m_FlJoint == nullptr) return;
		if (l_AIComponent->m_FrJoint == nullptr) return;

		const float lockAngle = 50 * DEGTORAD;
		const float turnSpeedPerSec = 160 * DEGTORAD;//from lock to lock in 0.5 sec
		const float turnPerTimeStep = turnSpeedPerSec / 60.0f;
		float desiredAngle = 0;

		AINode* currentNode = AINodeManager::GetSingleton().GetNode(l_AIComponent->GetCurrentNode());

		AINode* nextNode = AINodeManager::GetSingleton().GetNode(l_AIComponent->GetCurrentNode() + l_AIComponent->GetNodeReadSpeed());

		//calculate forward

		glm::vec2 forward = l_physics_component->CalculateForward();

		glm::vec2 dirToNextNode = nextNode->m_Position - l_TransformComponent->GetWorldLocation();
		dirToNextNode = glm::normalize(dirToNextNode);
		glm::vec2 leftOrRight = dirToNextNode - forward;

		int isRight = 0;

		if (leftOrRight.x < 0)
		{
			isRight = 1;
		}
		else if (leftOrRight.x > 0)
		{
			isRight = 2;
		}

		glm::vec2 die = forward + dirToNextNode;
		if (die.y > 0)
		{
			l_AIComponent->SetRestart(true);
		}

		glm::vec2 distTravelled;
		
		if(l_AIComponent->GetRestart())
		{
			currentNode->m_Speed = l_AIComponent->GetCurrentSpeed();
			currentNode->m_Direction = dirToNextNode;
			l_TransformComponent->SetZRotation(0);
			l_physics_component->GetPhysicsBody()->SetTransform(b2Vec2(0, 0), 0);
			l_TransformComponent->SetWorldLocation(glm::vec2(0));
			l_AIComponent->SetCurrentNode(0);
			l_AIComponent->SetRestart(false);
		}
		else
		{
			int index = 0;
			for (auto entity : GetEntityRegister().view<Tire, AITag>())
			{
				Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

				if (tirecomp->GetAIIndex() == l_AIComponent->GetAIIndex())
				{
					if (isRight == 1)
					{
						tirecomp->m_SteerDirection = SteerDirection::Right;
						desiredAngle = -lockAngle;
					}
					else if(isRight == 2)
					{
						tirecomp->m_SteerDirection = SteerDirection::Left;
						desiredAngle = lockAngle;
					}

					DoBehaviour(l_AIComponent, a_deltatime, tirecomp);

					float angleNow = l_AIComponent->m_FlJoint->GetJointAngle();
					float angleToTurn = desiredAngle - angleNow;
					angleToTurn = b2Clamp(angleToTurn, -turnPerTimeStep, turnPerTimeStep);
					float newAngle = angleNow + angleToTurn;
					l_AIComponent->m_FlJoint->SetLimits(newAngle, newAngle);
					l_AIComponent->m_FrJoint->SetLimits(newAngle, newAngle);

					distTravelled = l_TransformComponent->GetWorldLocation();
					
					glm::vec2 position = glm::vec2(0);
					position.x = l_physics_component->GetPhysicsBody()->GetPosition().x;
					position.y = l_physics_component->GetPhysicsBody()->GetPosition().y;

					l_TransformComponent->SetWorldLocation(position);

					UpdateDrift(l_physics_component, l_AIComponent);

					tirePosition[index].x = tirecomp->GetBody()->GetWorldCenter().x;
					tirePosition[index].y = tirecomp->GetBody()->GetWorldCenter().y;
					index++;
				}
			}			
		}

		for (int i = 0; i < 4; i++)
		{
			glm::vec2 positiontire = glm::vec2(0);
			positiontire.x = tirePosition[i].x;
			positiontire.y = tirePosition[i].y;

			renderer->MakePoint(positiontire.x, positiontire.y, 3.0f);

			glm::vec2 positiontire2 = glm::vec2(0);
			int temp = (i == 3 ? 0 : i + 1);
			positiontire2.x = tirePosition[temp].x;
			positiontire2.y = tirePosition[temp].y;
			renderer->MakeLine(positiontire.x, positiontire.y, positiontire2.x, positiontire2.y);
		}

		glm::vec2 DistTraveled = l_TransformComponent->GetWorldLocation() - distTravelled;
		float distance = DistTraveled.length();
		float lengthtrack = 0;
		for (auto entity : GetEntityRegister().view<SplineComponent, SplineTag>())
		{
			SplineComponent* TrackRefcomp = &GetEntityRegister().get<SplineComponent>(entity);
			lengthtrack = TrackRefcomp->GetLength();
		}

		float distTraveled = distance / lengthtrack;
		
		l_AIComponent->SetPositionOnSpline(distTraveled);
		if(l_AIComponent->GetPositionOnSpline() > currentNode->m_PosOnLine)
		{
			if (currentNode->m_Speed < l_AIComponent->GetCurrentSpeed())
			{
				currentNode->m_Speed = l_AIComponent->GetCurrentSpeed();
			}
			
			l_AIComponent->SetCurrentNode(l_AIComponent->GetCurrentNode() + l_AIComponent->GetNodeReadSpeed());
		}
	}
}

void AISystem::DoBehaviour(AIComponent* ai_component, float a_deltatime, Tire* tire)
{
	AINode* note = AINodeManager::GetSingleton().GetNode(ai_component->GetCurrentNode());
	if(note->m_Speed > ai_component->GetCurrentSpeed())
	{
		ai_component->SetAIState(AIState::Accelerate);
	}else if(note->m_Speed < ai_component->GetCurrentSpeed())
	{
		ai_component->SetAIState(AIState::Brake);
	}else
	{
		ai_component->SetAIState(AIState::Natural);
	}
	
	switch (ai_component->GetAIState())
	{
	case AIState::Accelerate:
	{
		ai_component->SetCurrentSpeed(ai_component->GetCurrentSpeed() + ai_component->GetAcceleration());
		
		ai_component->SetCurrentSpeed((ai_component->GetCurrentSpeed() + 10) / 100);
		if (ai_component->GetCurrentSpeed() > 300)
		{
			ai_component->SetCurrentSpeed(300.0f);
		}
		tire->SetDesiredSpeed(ai_component->GetCurrentSpeed());
		tire->m_DriveDirection = DriveDirection::Forward;
			
	}
	break;
	case AIState::Brake:
	{
		ai_component->SetCurrentSpeed(ai_component->GetCurrentSpeed() - ai_component->GetAcceleration());
		
		ai_component->SetCurrentSpeed((ai_component->GetCurrentSpeed() + 10) / 100);
		if (ai_component->GetCurrentSpeed() > 300)
		{
			ai_component->SetCurrentSpeed(300.0f);
		}
		tire->SetDesiredSpeed(ai_component->GetCurrentSpeed());
		tire->m_DriveDirection = DriveDirection::Backward;
	}
	break;
	case AIState::Natural:
	{
		//do nothing the car is moving in a normal speed.		
		tire->m_DriveDirection = DriveDirection::Neutral;		
	}
	break;
	}
}

void AISystem::UpdateDrift(PhysicsComponent* a_physics_component, AIComponent* a_ai_component)
{
	if (a_physics_component->GetDrift() < 1) {
		a_ai_component->m_ForwardSpeed = a_physics_component->GetForwardVelocityGLM();
		a_ai_component->m_LateralSpeed = a_physics_component->GetLateralVelocityGLM();
		if (glm::length(a_ai_component->m_LateralSpeed) < 1.0f) {
			KillDrift(a_physics_component, a_ai_component);
		}
		else {
			HandleDrift(a_physics_component, a_ai_component);
		}
	}
}

void AISystem::KillDrift(PhysicsComponent* a_physics_component, AIComponent* a_ai_component)
{
	a_physics_component->GetPhysicsBody()->SetLinearVelocity(a_physics_component->GetForwardVelocity());
}

void AISystem::HandleDrift(PhysicsComponent* a_physics_component, AIComponent* a_ai_component)
{
	b2Vec2 forwardSpeed = a_physics_component->GetForwardVelocity();
	b2Vec2 lateralSpeed = a_physics_component->GetLateralVelocity();
	float mDrift = 1.f;
	a_physics_component->GetPhysicsBody()->SetLinearVelocity(b2Vec2(forwardSpeed.x + lateralSpeed.x * mDrift, forwardSpeed.y + lateralSpeed.y * mDrift));
}

