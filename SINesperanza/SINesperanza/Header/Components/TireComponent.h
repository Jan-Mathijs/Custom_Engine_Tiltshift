#pragma once
#include <set>
#include "TireSupComponent.h"
#include "glm/glm/glm.hpp"
//#include "Core/WorldManager.h"
#include "Box2d/box2d.h"
#include "PhysicsComponent.h"

enum class DriveDirection
{
    Forward,
    Backward,
	Neutral,
};

enum class SteerDirection
{
    Left,
    Right,
	Nothing,
};

struct Tire
{
public:
    
    Tire() {
    	
    }

    ~Tire() {
    	if(m_body != nullptr)
    	{
            //m_body->GetWorld()->DestroyBody(m_body);
    	}        
    }

	void Initialize(b2World* theWorld, bool a_IsAI, int AIIndex)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        m_body = theWorld->CreateBody(&bodyDef);
       
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(0.5f, 1.25f);
        b2FixtureDef fixture;
        fixture.shape = &polygonShape;
        fixture.filter.maskBits;
        fixture.filter.categoryBits = PhysicsComponent::CollMasks::Tire;
        fixture.density = 1.f;
            
        m_body->CreateFixture(&fixture);//shape, density

        m_currentTraction = 1;
        m_currentDrag = 1;
        m_IsAI = a_IsAI;
        m_AIIndex = AIIndex;
    }
    b2Body* GetBody() { return m_body; }
    void SetBody(b2Body* a_body) { m_body = a_body; }

    float GetMaxForwardSpeed() { return m_maxForwardSpeed; }
    float GetMaxBackwardSpeed() { return m_maxBackwardSpeed; }
    float GetMaxDriveForce() { return m_maxDriveForce; }
    float GetMaxLateralImpulse() { return m_maxLateralImpulse; }
    float GetCurrentTraction() { return m_currentTraction; }
    float GetCurrentDrag() { return GetCurrentDrag(); }
    float GetDesiredSpeed() { return m_DesiredSpeed; }
    int GetAIIndex() { return m_AIIndex; }
    bool GetIsAI() { return m_IsAI; }
    void SetMaxForwardSpeed(float a_MaxForward) { m_maxForwardSpeed = a_MaxForward; }
    void SetMaxBackwardSpeed(float a_backward) { m_maxBackwardSpeed = a_backward; }
    void SetMaxDriveForce(float a_maxDriveForce) { m_maxDriveForce = a_maxDriveForce; }
    void SetMaxLateralImpulse(float a_maxLateralImpulse) { m_maxLateralImpulse = a_maxLateralImpulse; }

    void SetCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse)
    {
        SetMaxForwardSpeed(maxForwardSpeed);
        SetMaxBackwardSpeed(maxBackwardSpeed);
        SetMaxDriveForce(maxDriveForce);
        SetMaxLateralImpulse(maxLateralImpulse);
    }
    void SetDesiredSpeed(float a_speed) { m_DesiredSpeed = a_speed; }
    SteerDirection m_SteerDirection = SteerDirection::Nothing;
    DriveDirection m_DriveDirection = DriveDirection::Neutral;
	
private:
	
	b2Body* m_body = nullptr;
    float m_maxForwardSpeed = 100.0f;
    float m_maxBackwardSpeed = -20.f;
    float m_maxDriveForce = 150.0f;
    float m_maxLateralImpulse = 0;
    std::set<GroundAreaFUDR*> m_groundAreas{nullptr};
    float m_currentTraction = 0;
    float m_currentDrag = 0;

    float m_lastDriveImpulse = 0;
    float m_lastLateralFrictionImpulse = 0;
    float m_DesiredSpeed = 0;

    bool m_IsAI = false;
    int m_AIIndex = -1;
};

