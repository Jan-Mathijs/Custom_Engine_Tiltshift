#include "AICarSystem.h"
#include "Core/WorldManager.h"
#include "Components/TireComponent.h"
#include "Components/TransformComponent.h"
#include "Components/AIComponent.h"
#include "Components/SplineComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/Tags.h"
#include "Components/SpriteComponent.h"
#include "Graphics/Sprite.h"

AICarSystem::AICarSystem()
{
	Register();
	CreateCar();
	CreateCar();
	CreateCar();
	CreateCar();
	CreateCar();
	CreateCar();
}

AICarSystem::~AICarSystem()
{
	
}

void AICarSystem::CreateCar()
{
	entt::entity Tire1 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(Tire1);
	GetEntityRegister().emplace<AITag>(Tire1);
	Tire& tire1ref = GetEntityRegister().emplace<Tire>(Tire1);
	tire1ref.Initialize(&SIN::WorldManager::GetSingleton().PhysicsWorld, true, m_AICount);

	entt::entity Tire2 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(Tire2);
	GetEntityRegister().emplace<AITag>(Tire2);
	Tire& tire2ref = GetEntityRegister().emplace<Tire>(Tire2);
	tire2ref.Initialize(&SIN::WorldManager::GetSingleton().PhysicsWorld, true, m_AICount);

	entt::entity Tire3 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(Tire3);
	GetEntityRegister().emplace<AITag>(Tire3);
	Tire& tire3ref = GetEntityRegister().emplace<Tire>(Tire3);
	tire3ref.Initialize(&SIN::WorldManager::GetSingleton().PhysicsWorld, true, m_AICount);

	entt::entity Tire4 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(Tire4);
	GetEntityRegister().emplace<AITag>(Tire4);
	Tire& tire4ref = GetEntityRegister().emplace<Tire>(Tire4);
	tire4ref.Initialize(&SIN::WorldManager::GetSingleton().PhysicsWorld, true, m_AICount);

	entt::entity Car = CreateEntity();
	SIN::TransformComponent* transcomp = &GetEntityRegister().emplace<SIN::TransformComponent>(Car);
	transcomp->SetWorldLocation(glm::vec2(-30, -15));
	transcomp->SetScale(glm::vec2(6.0f));
	SIN::AIComponent* aicomp = &GetEntityRegister().emplace<SIN::AIComponent>(Car);
	aicomp->SetAIIndex(m_AICount);
	GetEntityRegister().emplace<AITag>(Car);
	std::string tempstr = "Assets/Textures/car.png";
	SpriteTag* spritetag = &GetEntityRegister().emplace<SpriteTag>(Car, tempstr);
	GetEntityRegister().emplace<SIN::SpriteComponent>(Car, tempstr);
	PhysicsComponent& physics_component = GetEntityRegister().emplace<PhysicsComponent>(Car);

	//aicomp->m_Tires = m_Tires;
	aicomp->m_FlJoint = flJoint;
	aicomp->m_FrJoint = frJoint;

	for (auto entity : GetEntityRegister().view<PhysicsComponent, AITag, SIN::AIComponent>())
	{
		SIN::AIComponent* aicomp = &GetEntityRegister().get<SIN::AIComponent>(entity);
		if(aicomp->GetAIIndex() == m_AICount)
		{
			PhysicsComponent* physics_component = &GetEntityRegister().get<PhysicsComponent>(entity);
			physics_component->SetPhysicsWorld(&SIN::WorldManager::GetSingleton().PhysicsWorld);
		}
	}

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	physics_component.SetPhysicsBody(SIN::WorldManager::GetSingleton().PhysicsWorld.CreateBody(&bodyDef));
	physics_component.GetPhysicsBody()->SetAngularDamping(3);

	b2Vec2 vertices[8];
	vertices[0].Set(1.5f, 0.f);
	vertices[1].Set(3.f, 2.5f);
	vertices[2].Set(2.8f, 5.5f);
	vertices[3].Set(1.f, 10.f);
	vertices[4].Set(-1.f, 10.f);
	vertices[5].Set(-2.8f, 5.5f);
	vertices[6].Set(-3.f, 2.5f);
	vertices[7].Set(-1.5f, 0.f);

	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 8);
	b2FixtureDef fixture;
	fixture.shape = &polygonShape;
	fixture.filter.categoryBits = PhysicsComponent::CollMasks::AICar;
	fixture.filter.maskBits = PhysicsComponent::CollMasks::Wall;
	fixture.density = 0.1f;
	physics_component.GetPhysicsBody()->CreateFixture(&fixture);

	b2RevoluteJointDef jointDef;
	jointDef.bodyA = physics_component.GetPhysicsBody();
	jointDef.enableLimit = true;
	jointDef.lowerAngle = 0;//with both these at zero...
	jointDef.upperAngle = 0;//...the joint will not move
	jointDef.localAnchorB.SetZero();

	float maxForwardSpeed = 250;
	float maxBackwardSpeed = -40;
	float backTireMaxDriveForce = 300;
	float frontTireMaxDriveForce = 500;
	float backTireMaxLateralImpulse = 8.5;
	float frontTireMaxLateralImpulse = 7.5;

	Tire* tire1comp = &GetEntityRegister().get<Tire>(Tire1);
	tire1comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire1comp->GetBody();
	jointDef.localAnchorA.Set(-3, 0.75f);
	SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

	Tire* tire2comp = &GetEntityRegister().get<Tire>(Tire2);
	tire2comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire2comp->GetBody();
	jointDef.localAnchorA.Set(3, 0.75f);
	SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

	Tire* tire3comp = &GetEntityRegister().get<Tire>(Tire3);
	tire3comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire3comp->GetBody();
	jointDef.localAnchorA.Set(-3, 8.5f);
	flJoint = (b2RevoluteJoint*)SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);
	aicomp->m_FlJoint = flJoint;
	SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

	Tire* tire4comp = &GetEntityRegister().get<Tire>(Tire4);
	tire4comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire4comp->GetBody();
	jointDef.localAnchorA.Set(3, 8.5f);
	frJoint = (b2RevoluteJoint*)SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);
	aicomp->m_FrJoint = frJoint;
	SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

	physics_component.GetPhysicsBody()->SetTransform(b2Vec2(0, 0), 0);

	m_AICount++;
}

void AICarSystem::OnUpdate(float a_DeltaTime)
{
	int total = 0;
	int minimumAI = 5;
	for (auto entity : GetEntityRegister().view<SIN::AIComponent, AITag, PhysicsComponent>())
	{
		total++;
	}

	if(total < minimumAI)
	{
		int temp = minimumAI - total;
		for(int i = 0; i < temp; i++ )
		{
			
		}
		//	CreateCar();
	}
	
}



