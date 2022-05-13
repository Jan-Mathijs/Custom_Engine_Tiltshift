#pragma once
#pragma warning(push)
#pragma warning(disable : 26812 26495)
#include <Box2d/b2_world.h>
#include "Box2d/box2d.h"
#pragma warning(pop)
#include <glm/glm/vec2.hpp>

struct PhysicsComponent
{
public:

	enum CollMasks
	{
		General = 0x0001,
		Car = 0x0002,
		Wall = 0x0003,
		AICar = 0x0004,
		Tire = 0x0005,
	};
	
	b2World* GetPhysicsWorld() { return m_PhysicsWorld; }
	b2Body* GetPhysicsBody() { return m_PhysicsBody; }
	void SetPhysicsWorld(b2World* a_physicsWorld) { m_PhysicsWorld = a_physicsWorld; }
	void SetPhysicsBody(b2Body* a_physiscsBody) { m_PhysicsBody = a_physiscsBody; }
	int GetDrift() { return m_Drift; }
	
	glm::vec2 CalculateForward()
	{
		float angle = GetPhysicsBody()->GetAngle();
		glm::vec2 forward = glm::vec2(0, 1);
		forward.x = cos(angle) * forward.x; 
		forward.y = sin(angle) * forward.y;
		return forward;
	}

	glm::vec2 GetForwardVelocityGLM()
	{
		b2Vec2 currentNormalB2 = m_PhysicsBody->GetWorldVector(b2Vec2(0, 1));
		b2Vec2 b2LinearVelocity = m_PhysicsBody->GetLinearVelocity();
		float dotb2 = b2Dot(currentNormalB2, b2LinearVelocity);
		return glm::vec2(dotb2 * currentNormalB2.x, dotb2 * currentNormalB2.y);	
	}
	
	b2Vec2 GetForwardVelocity()
	{
		b2Vec2 currentNormalB2 = m_PhysicsBody->GetWorldVector(b2Vec2(0, 1));
		b2Vec2 b2LinearVelocity = m_PhysicsBody->GetLinearVelocity();		
		float dotb2 = b2Dot(currentNormalB2, b2LinearVelocity);		
		return b2Vec2(dotb2 * currentNormalB2.x, dotb2 * currentNormalB2.y);
	}
	
	b2Vec2 GetLateralVelocity()
	{
		b2Vec2 currentNormal = m_PhysicsBody->GetWorldVector(b2Vec2(1, 0));
		float dotProduct = b2Dot(currentNormal, m_PhysicsBody->GetLinearVelocity());
		b2Vec2 temp = b2Vec2(dotProduct * currentNormal.x, dotProduct * currentNormal.y);
		return temp;
	}
	
	glm::vec2 GetLateralVelocityGLM()
	{
		b2Vec2 currentNormal = m_PhysicsBody->GetWorldVector(b2Vec2(1, 0));
		float dotProduct = b2Dot(currentNormal, m_PhysicsBody->GetLinearVelocity());
		b2Vec2 temp = b2Vec2(dotProduct * currentNormal.x, dotProduct * currentNormal.y);
		glm::vec2 glm;
		glm.x = temp.x;
		glm.y = temp.y;
		return glm;
	}

private:
	b2World* m_PhysicsWorld = nullptr;
	b2Body* m_PhysicsBody = nullptr;
	int m_Drift = 0;
};