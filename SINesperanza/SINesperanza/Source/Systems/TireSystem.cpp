#include "Systems/TireSystem.h"
using namespace SIN;

TireSystem::TireSystem()
{
	Register();
	//CreateEntities();
}

TireSystem::~TireSystem()
{
	
}


void TireSystem::OnUpdate(float a_Deltatime)
{
	for (auto entity : GetEntityRegister().view<Tire>())
	{
		Tire* tireComponent = &GetEntityRegister().get<Tire>(entity);
				
		UpdateFriction(tireComponent);
		UpdateDrive(tireComponent);		
		UpdateTurn(tireComponent);		
	}

}

void TireSystem::UpdateFriction(Tire* a_tire_component)
{
	float maxLateralImpulse = 2.5f;
	b2Vec2 impulse = a_tire_component->GetBody()->GetMass() * -getLateralVelocityB2Vec(a_tire_component);
	if (impulse.Length() > maxLateralImpulse)
		impulse *= maxLateralImpulse / impulse.Length();
	a_tire_component->GetBody()->ApplyLinearImpulse(impulse, a_tire_component->GetBody()->GetWorldCenter(), true);

	//angular velocity
	a_tire_component->GetBody()->ApplyAngularImpulse(0.1f * a_tire_component->GetBody()->GetInertia() * -a_tire_component->GetBody()->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = GetForwardVelocity(a_tire_component);
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	a_tire_component->GetBody()->ApplyForce(dragForceMagnitude * currentForwardNormal, a_tire_component->GetBody()->GetWorldCenter(), true);
}


void TireSystem::UpdateDrive(Tire* a_tire_component)
{
	float desiredSpeed = 0;

	switch (a_tire_component->m_DriveDirection)
	{
	case DriveDirection::Forward:
		{
			if(a_tire_component->GetIsAI())
			{
				desiredSpeed = a_tire_component->GetDesiredSpeed();
			}else
			{
				desiredSpeed = a_tire_component->GetMaxForwardSpeed();
			}
			
		}
		break;
	case DriveDirection::Backward:
		{
			if (a_tire_component->GetIsAI())
			{
				desiredSpeed = a_tire_component->GetDesiredSpeed();
			}
			else
			{
				desiredSpeed = a_tire_component->GetMaxBackwardSpeed();
			}
		}
		break;
	case DriveDirection::Neutral:
		break;
	default: break;
	}
	
	//find current speed in forward direction
	b2Vec2 currentForwardNormal = a_tire_component->GetBody()->GetWorldVector(b2Vec2(0, 1));
	float currentSpeed = b2Dot(getLateralVelocityB2Vec(a_tire_component), currentForwardNormal);

	//apply necessary force
	float force = 0;
	if (desiredSpeed > currentSpeed)
		force = a_tire_component->GetMaxDriveForce();
	else if (desiredSpeed < currentSpeed)
		force = -a_tire_component->GetMaxDriveForce();
	else
		return;
	a_tire_component->GetBody()->ApplyForce(force * currentForwardNormal, a_tire_component->GetBody()->GetWorldCenter(), true);
}

void TireSystem::UpdateTurn(Tire* a_tire_component)
{	
	float desiredTorque = 0;
	switch (a_tire_component->m_SteerDirection)
	{
	case SteerDirection::Left:
		{
		desiredTorque = 15; 
		}
		break;
	case SteerDirection::Right:
		{
		desiredTorque = -15; 
		}
		break;
	case SteerDirection::Nothing:
		break;
	default: break;//nothing
	}
	
	a_tire_component->GetBody()->ApplyTorque(desiredTorque, true);	
}

b2Vec2 TireSystem::getLateralVelocityB2Vec(Tire* a_tire_component)
{
	b2Vec2 currentRightNormal = a_tire_component->GetBody()->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, a_tire_component->GetBody()->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 TireSystem::GetForwardVelocity(Tire* a_tire_component)
{
	b2Vec2 currentForwardNormal = a_tire_component->GetBody()->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, a_tire_component->GetBody()->GetLinearVelocity()) * currentForwardNormal;
}


