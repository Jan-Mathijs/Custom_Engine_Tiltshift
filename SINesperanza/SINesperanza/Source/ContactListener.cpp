#include "ContactListener.h"
#include "Core/WorldManager.h"
#include "Components/Tags.h"
// figure out if one of the colliding entities is a sensor and which one that is


	//Rewrite this for this engine if you want to have sensors
    bool MyContactListener::GetSensorAndHitEnt(b2Contact* contact, uint64_t& SensorOwner, uint64_t& HitEntity)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();


        //make sure only one of the fixtures was a sensor
        bool sensorA = fixtureA->IsSensor();
        bool sensorB = fixtureB->IsSensor();
        if (!(sensorA ^ sensorB))
            return false;
        return true;
    }



    void MyContactListener::BeginContact(b2Contact* contact)
    {
        //printf("A collision happened\n");
       
    	
    	for(auto entity : SIN::WorldManager::GetSingleton().GetEntityRegister().view<PhysicsComponent, SIN::AIComponent, SIN::TransformComponent, AITag>())
    	{
            PhysicsComponent* physics_component = &SIN::WorldManager::GetSingleton().GetEntityRegister().get<PhysicsComponent>(entity);
            SIN::AIComponent* ai_component = &SIN::WorldManager::GetSingleton().GetEntityRegister().get<SIN::AIComponent>(entity);
            SIN::TransformComponent* transform_component = &SIN::WorldManager::GetSingleton().GetEntityRegister().get<SIN::TransformComponent>(entity);

			b2Vec2 pos1 =  contact->GetFixtureA()->GetBody()->GetPosition();    		
    		        
            if (transform_component->GetWorldLocation().x == pos1.x && transform_component->GetWorldLocation().y == pos1.y)
            {
              //  ai_component->SetRestart(true);
                return;
            }
            
            b2Vec2 pos2 = contact->GetFixtureB()->GetBody()->GetPosition();
            if (transform_component->GetWorldLocation().x == pos2.x && transform_component->GetWorldLocation().y == pos2.y)
            {
                return;
            }          
    	}      
    }

    void MyContactListener::EndContact(b2Contact* contact)
    {
        uint64_t SensorOwner;
        uint64_t HitEntity;
    }


