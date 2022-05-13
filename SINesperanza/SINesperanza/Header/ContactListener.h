#pragma once
#include "box2d/b2_contact.h"
#include "Box2d/b2_world_callbacks.h"
#include "Core/Systems.h"
#include "Components/PhysicsComponent.h"

	class MyContactListener : public b2ContactListener
	{
		virtual void BeginContact(b2Contact* contact) override;
		virtual void EndContact(b2Contact* contact) override;

		bool GetSensorAndHitEnt(b2Contact* contact, uint64_t& SensorOwner, uint64_t& HitEntity);
	};

