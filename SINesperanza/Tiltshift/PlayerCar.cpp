#include "PlayerCar.h"
#include "Components/SpriteComponent.h"
#include "Components/OrthoCameraComponent.h"
#include "Components/TireComponent.h"
#include "Graphics/Renderer.h"
#include "Components/Tags.h"
#include "Core/WorldManager.h"
#include "Input/Input.h"

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

PlayerCarSystem::PlayerCarSystem()
{
	Register();
	CreateCar();
}

PlayerCarSystem::~PlayerCarSystem()
{
	
}


void PlayerCarSystem::CreateCar()
{
	
	entt::entity PlayerTire1 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(PlayerTire1);
	GetEntityRegister().emplace<PlayerTag>(PlayerTire1);
	GetEntityRegister().emplace<Tire>(PlayerTire1);
	
	//entt::registry<PlayerTag>
	entt::entity PlayerTire2 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(PlayerTire2);
	GetEntityRegister().emplace<PlayerTag>(PlayerTire2);
	GetEntityRegister().emplace<Tire>(PlayerTire2);
		
	entt::entity PlayerTire3 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(PlayerTire3);
	GetEntityRegister().emplace<PlayerTag>(PlayerTire3);
	GetEntityRegister().emplace<Tire>(PlayerTire3);
	
	entt::entity PlayerTire4 = CreateEntity();
	GetEntityRegister().emplace<SIN::TransformComponent>(PlayerTire4);
	GetEntityRegister().emplace<PlayerTag>(PlayerTire4);
	GetEntityRegister().emplace<Tire>(PlayerTire4);
		
	entt::entity PlayerCar = CreateEntity();
	SIN::TransformComponent* playertrans = &GetEntityRegister().emplace<SIN::TransformComponent>(PlayerCar);
	playertrans->SetZRotation(0.0f);
	playertrans->SetWorldLocation(glm::vec2(0, 0));
	GetEntityRegister().emplace<PlayerTag>(PlayerCar);
	PhysicsComponent* physics_component = &GetEntityRegister().emplace<PhysicsComponent>(PlayerCar);
	SpriteTag* spritetag = &GetEntityRegister().emplace<SpriteTag>(PlayerCar, "Assets/Textures/car.png");
	//spritetag->m_FileName = "Assets/Textures/car.png";
//	std::string tempstr = "Assets/Textures/car.png";
	//GetEntityRegister().emplace<SIN::SpriteComponent>(PlayerCar, tempstr);
	
	playertrans->SetScale(glm::vec2(9.f,10.0f));
	
	entt::entity camera = CreateEntity();
	SIN::TransformComponent* transformcomp = &GetEntityRegister().emplace<SIN::TransformComponent>(camera);
	GetEntityRegister().emplace<PlayerTag>(camera);
	transformcomp->SetZRotation(0);
	transformcomp->SetWorldLocation(playertrans->GetWorldLocation());
	GetEntityRegister().emplace<SIN::OrthoCameraComponent>(camera, 288.f, 192.f);
	
	for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
	{
		Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);
		tirecomp->Initialize(&SIN::WorldManager::GetSingleton().PhysicsWorld, false, -2);
		m_Tires.push_back(tirecomp);
		m_tireBodies.push_back(m_Tires.at(m_Tires.size() - 1)->GetBody());
	}

	for (auto entity : GetEntityRegister().view<PhysicsComponent, PlayerTag>())
	{
		PhysicsComponent* physics_component = &GetEntityRegister().get<PhysicsComponent>(entity);

		physics_component->SetPhysicsWorld(&SIN::WorldManager::GetSingleton().PhysicsWorld);
	}

	for (auto entity3 : GetEntityRegister().view<PlayerTag, SIN::TransformComponent, PhysicsComponent>())
	{
		PhysicsComponent* physics_component2 = &GetEntityRegister().get<PhysicsComponent>(entity3);
		SIN::TransformComponent* transform = &GetEntityRegister().get<SIN::TransformComponent>(entity3);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = b2Vec2(transform->GetWorldLocation().x, transform->GetWorldLocation().y);
		physics_component2->SetPhysicsBody(SIN::WorldManager::GetSingleton().PhysicsWorld.CreateBody(&bodyDef));
		physics_component2->GetPhysicsBody()->SetAngularDamping(3);	
	
		b2Vec2 vertices[8];
		vertices[0].Set(1.5, 0);
		vertices[1].Set(3, 2.5);
		vertices[2].Set(2.8, 5.5);
		vertices[3].Set(1, 10);
		vertices[4].Set(-1, 10);
		vertices[5].Set(-2.8, 5.5);
		vertices[6].Set(-3, 2.5);
		vertices[7].Set(-1.5, 0);

		b2PolygonShape polygonShape;
		polygonShape.Set(vertices, 8);
		b2FixtureDef fixture;
		fixture.shape = &polygonShape;
		fixture.filter.categoryBits = PhysicsComponent::CollMasks::AICar;
		fixture.filter.maskBits =  PhysicsComponent::CollMasks::Wall; //add general for collision to AI
		fixture.density = 0.1f;
		physics_component2->GetPhysicsBody()->CreateFixture(&fixture);
		//= physics_component2->GetPhysicsBody()->CreateFixture(&polygonShape, 0.1f);//shape, density
		
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = physics_component2->GetPhysicsBody();
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

		Tire* tire1comp = &GetEntityRegister().get<Tire>(PlayerTire1);
		tire1comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
		jointDef.bodyB = tire1comp->GetBody();
		jointDef.localAnchorA.Set(-3, 0.75f);
		SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

		Tire* tire2comp = &GetEntityRegister().get<Tire>(PlayerTire2);
		tire2comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
		jointDef.bodyB = tire2comp->GetBody();
		jointDef.localAnchorA.Set(3, 0.75f);
		SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

		Tire* tire3comp = &GetEntityRegister().get<Tire>(PlayerTire3);
		tire3comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
		jointDef.bodyB = tire3comp->GetBody();
		jointDef.localAnchorA.Set(-3, 8.5f);
		m_FlJoint = (b2RevoluteJoint*)SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);
		SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);

		Tire* tire4comp = &GetEntityRegister().get<Tire>(PlayerTire4);
		tire4comp->SetCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
		jointDef.bodyB = tire4comp->GetBody();
		jointDef.localAnchorA.Set(3, 8.5f);
		m_FrJoint = (b2RevoluteJoint*)SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);
		SIN::WorldManager::GetSingleton().PhysicsWorld.CreateJoint(&jointDef);
	}
}



void PlayerCarSystem::OnUpdate(float a_DeltaTime)
{
	RegisterInput();
	
	float lockAngle = 50 * DEGTORAD;
	float turnSpeedPerSec = 160 * DEGTORAD;//from lock to lock in 0.5 sec
	float turnPerTimeStep = turnSpeedPerSec / 60.0f;
	float desiredAngle = 0;

	switch(m_Tires.at(0)->m_SteerDirection)
	{
	case SteerDirection::Left:
		{
			desiredAngle = lockAngle;
		}
		break;
	case SteerDirection::Right:
		{
			desiredAngle = -lockAngle;
		}
		break;
	default: break;
	}
	
	float angleNow = m_FlJoint->GetJointAngle();
	float angleToTurn = desiredAngle - angleNow;
	angleToTurn = b2Clamp(angleToTurn, -turnPerTimeStep, turnPerTimeStep);
	float newAngle = angleNow + angleToTurn;
	m_FlJoint->SetLimits(newAngle, newAngle);
	m_FrJoint->SetLimits(newAngle, newAngle);

	SIN::Renderer* renderer = SIN::WorldManager::GetSingleton().GetRenderer();
	
	for (auto entity : GetEntityRegister().view<PhysicsComponent, SIN::TransformComponent, PlayerTag>())
	{
		PhysicsComponent* physics_component = &GetEntityRegister().get<PhysicsComponent>(entity);
		SIN::TransformComponent* transform_component = &GetEntityRegister().get<SIN::TransformComponent>(entity);
		
		
		glm::vec2 tirePosition[4];
		int index2 = 0;
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirePosition[index2].x = tirecomp->GetBody()->GetPosition().x;
				tirePosition[index2].y = tirecomp->GetBody()->GetPosition().y;
				index2++;
			}
		}
		
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 positiontire;
			positiontire.x = tirePosition[i].x;
			positiontire.y = tirePosition[i].y;

			renderer->MakePoint(positiontire.x, positiontire.y, 3.0f);

			glm::vec2 positiontire2;
			int temp = (i == 3 ? 0 : i + 1);
			positiontire2.x = tirePosition[temp].x;
			positiontire2.y = tirePosition[temp].y;
			renderer->MakeLine(positiontire.x, positiontire.y, positiontire2.x, positiontire2.y);
		}
		
		UpdateDrift(physics_component);
		
		glm::vec2 position;
		position.x = physics_component->GetPhysicsBody()->GetPosition().x;
		position.y = physics_component->GetPhysicsBody()->GetPosition().y;

		transform_component->SetWorldLocation(position);

		transform_component->SetZRotation(physics_component->GetPhysicsBody()->GetAngle() * RADTODEG);
		
		UpdateCamera(physics_component, transform_component, a_DeltaTime);		
	}
}

void PlayerCarSystem::UpdateCamera(PhysicsComponent* a_physics_component, SIN::TransformComponent* a_transform_component, float a_Deltatime)
{
	for (auto entity2 : GetEntityRegister().view<PlayerTag, SIN::TransformComponent, SIN::OrthoCameraComponent>())
	{
		SIN::OrthoCameraComponent* cameracomp = &GetEntityRegister().get<SIN::OrthoCameraComponent>(entity2);
		SIN::TransformComponent* transform_componentcamera = &GetEntityRegister().get<SIN::TransformComponent>(entity2);

		glm::vec2 velocitycar;
		velocitycar.x = a_physics_component->GetPhysicsBody()->GetLinearVelocity().x;
		velocitycar.y = a_physics_component->GetPhysicsBody()->GetLinearVelocity().y;

		glm::vec2 cameraWantToGoPos = a_transform_component->GetWorldLocation() + velocitycar * m_CameraLerpScale;
		m_CurrentCameraPos += (cameraWantToGoPos - m_CurrentCameraPos) * a_Deltatime * 4.5f;

		m_CurrentCameraRotation += (a_transform_component->GetZRotation() - m_CurrentCameraRotation) * a_Deltatime * m_CameraLerpPositionScale;

		transform_componentcamera->SetWorldLocation(glm::vec2(m_CurrentCameraPos.x, m_CurrentCameraPos.y));
		transform_componentcamera->SetZRotation(m_CurrentCameraRotation);

		float scaler = m_ForwardSpeed.length() / 2;
		float afterscale = scaler / 1.7f; 
		float screenx = afterscale * 288.f;
		float screeny = afterscale *  192.f;
		if(screenx >= 375.f)
		{
			screenx = 375.f;
		}else if(screenx <= 288.f)
		{
			screenx = 288.f;
		}

		if (screeny >= 250.f)
		{
			screeny = 250.f;
		}
		else if (screeny <= 192)
		{
			screeny = 192;
		}
		
		cameracomp->SetViewBounds(screenx, screeny);
		
		//float zoomout = a_physics_component->GetPhysicsBody()->GetLinearVelocity().Length();
	}
}



void PlayerCarSystem::RegisterInput()
{
	if (INPUT.IsKeyPressed(SIN_KEY_W))
	{
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirecomp->m_DriveDirection = DriveDirection::Forward;
			}
		}
	}else if (INPUT.IsKeyPressed(SIN_KEY_S))
	{
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirecomp->m_DriveDirection = DriveDirection::Backward;
			}
		}
	}else if(!INPUT.IsKeyPressed(SIN_KEY_S) || !INPUT.IsKeyPressed(SIN_KEY_W))
	{
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirecomp->m_DriveDirection = DriveDirection::Neutral;
			}
		}
	}

	if (INPUT.IsKeyPressed(SIN_KEY_A))
	{
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirecomp->m_SteerDirection = SteerDirection::Left;
			}
		}
	}
	else if (INPUT.IsKeyPressed(SIN_KEY_D))
	{
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirecomp->m_SteerDirection = SteerDirection::Right;
			}
		}
	}
	else if (!INPUT.IsKeyPressed(SIN_KEY_D) || !INPUT.IsKeyPressed(SIN_KEY_A))
	{
		int index = 0;
		for (auto entity : GetEntityRegister().view<Tire, PlayerTag>())
		{
			Tire* tirecomp = &GetEntityRegister().get<Tire>(entity);

			if (tirecomp->GetAIIndex() == -2)
			{
				tirecomp->m_SteerDirection = SteerDirection::Nothing;
			}
		}
	}
}

void PlayerCarSystem::HandleDrift(PhysicsComponent* a_physics_component)
{
	b2Vec2 forwardSpeed = a_physics_component->GetForwardVelocity();
	b2Vec2 lateralSpeed = a_physics_component->GetLateralVelocity();
	float mDrift = 1.f;
	a_physics_component->GetPhysicsBody()->SetLinearVelocity(b2Vec2(forwardSpeed.x + lateralSpeed.x * mDrift, forwardSpeed.y + lateralSpeed.y * mDrift));
}

void PlayerCarSystem::KillDrift(PhysicsComponent* a_physics_component)
{
	a_physics_component->GetPhysicsBody()->SetLinearVelocity(a_physics_component->GetForwardVelocity());
}

void PlayerCarSystem::UpdateDrift(PhysicsComponent* a_physics_component)
{
	if (a_physics_component->GetDrift() < 1) {
		m_ForwardSpeed = a_physics_component->GetForwardVelocityGLM();
		m_LateralSpeed = a_physics_component->GetLateralVelocityGLM();
		if (glm::length(m_LateralSpeed) < 1.0f ) {
			KillDrift(a_physics_component);
		}
		else {
			HandleDrift(a_physics_component);
		}
	}
}

