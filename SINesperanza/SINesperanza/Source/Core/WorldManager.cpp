#include "Core/WorldManager.h"
#include "Components/OrthoCameraComponent.h"
#include "Systems/AISystem.h"
#include "Systems/TireSystem.h"
#include "Core/AINodeManager.h"
#include "Components/Tags.h"
#include "Components/SpriteComponent.h"
//#include "cereal/types/unordered_map.hpp"
#include "Core/FileLoader.h"
#ifdef PLATFORM_WIN64
#include "Graphics/GLRenderer.h"
#include "Graphics/Window.h"
#endif

#define PI  3.14159265359
#define HALF_PI PI/2.0

namespace SIN
{
	WorldManager::WorldManager() :
		m_AISystem(nullptr), //Otherwise VS compiler complains
		m_Renderer(nullptr),
		m_SplineSystem(nullptr),
		m_TireSystem(nullptr)
	{
		
	}

	WorldManager::~WorldManager()
	{
		
	}

	int32_t id;
	std::shared_ptr<std::unordered_map<uint32_t, AINode>> data;
	template <class Archive>
	void save(Archive& ar) 
	{
		ar(data);
	}

	template <class Archive>
	void load(Archive& ar)
	{
		static int32_t idGen = 0;
		id = idGen++;
		ar(data);
	}

	void WorldManager::Initialize()
	{
		m_Systems = std::vector<Systems*>(0);
		m_AISystem = new AISystem();
		m_SplineSystem = new SplineSystem();
		m_TireSystem = new TireSystem();
		
		GenerateTrack();
				
#ifdef PLATFORM_WIN64
		m_Renderer = new GLRenderer();
#endif
		PhysicsWorld.SetContactListener(&myContactListenerInstance);
		entt::entity StartScreenentt = CreateEntity();
		TransformComponent* trans = &GetEntityRegister().emplace<TransformComponent>(StartScreenentt);
		GetEntityRegister().emplace<StartScreen>(StartScreenentt);
		trans->SetWorldLocation(glm::vec2(0));
		trans->SetZRotation(0);
		trans->SetScale(glm::vec2(350));
		GetEntityRegister().emplace<SpriteTag>(StartScreenentt, m_StartScreenFileName);
		GetEntityRegister().emplace<SIN::SpriteComponent>(StartScreenentt, m_StartScreenFileName);
		GetEntityRegister().emplace<OrthoCameraComponent>(StartScreenentt, 288.f, 192.f);
	}

	WorldManager& WorldManager::GetSingleton()
	{
		static WorldManager singleton;
		return singleton;
	}

	Renderer* WorldManager::GetRenderer()
	{
		return m_Renderer;
	}

	void WorldManager::RegisterSystem(Systems* a_System)
	{
		m_Systems.push_back(a_System);
	}

	entt::registry& WorldManager::GetEntityRegister()
	{
		return m_Register;
	}

	entt::entity WorldManager::CreateEntity()
	{
		return GetEntityRegister().create();
	}

	void WorldManager::DeleteEntity(entt::entity a_Entity)
	{
		m_Register.destroy(a_Entity);
	}

	void WorldManager::RemoveAllComponents(entt::entity a_Entity)
	{
		m_Register.remove_all(a_Entity);
	}

	void WorldManager::Update(float a_DeltaTime)
	{
		// Physics system.

		// Gameplay systems.
		for (size_t i = 0; i < m_Systems.size(); i++)
		{
			//printf("Tick");
			m_Systems[i]->OnUpdate(a_DeltaTime);
		}

		Render(1);
	}

	void WorldManager::GenerateTrack()
	{

		if(AINodeManager::GetSingleton().IsTheMapLoaded())
		{
			//entt::entity Track = CreateEntity();
		//	SplineComponent* TrackRef = &GetEntityRegister().emplace<SplineComponent>(Track);
			//GetEntityRegister().emplace<SplineTag>(Track);			
		}else
		{
			//different track
			/*entt::entity Track = CreateEntity();
			SplineComponent* TrackRef = &GetEntityRegister().emplace<SplineComponent>(Track);
			GetEntityRegister().emplace<SplineTag>(Track);

			TrackRef->SetType(SplineType::Bezier);
			TrackRef->AddPoint(glm::vec2(1500, 2000));
			TrackRef->AddPoint(glm::vec2(-1000, 1600));
			TrackRef->AddPoint(glm::vec2(-1700, 1000));
			TrackRef->AddPoint(glm::vec2(-1200, -600));
			TrackRef->AddPoint(glm::vec2(0, 0));
			TrackRef->MovePoint(0, glm::vec2(0));
			TrackRef->MovePoint(1, glm::vec2(1700, 1400));
			TrackRef->MovePoint(3, glm::vec2(-200, 2100));
			TrackRef->MovePoint(5, glm::vec2(-2000, 1400));
			TrackRef->MovePoint(7, glm::vec2(-900, -400));*/

			entt::entity Track2 = CreateEntity();
			SplineComponent* TrackRef2 = &GetEntityRegister().emplace<SplineComponent>(Track2);
			GetEntityRegister().emplace<SplineTag>(Track2);

			TrackRef2->SetType(SplineType::Bezier);
			TrackRef2->AddPoint(glm::vec2(1500, 0));
			TrackRef2->AddPoint(glm::vec2(1500, 2000));
			TrackRef2->AddPoint(glm::vec2(0, 1000));
			//TrackRef->AddPoint(glm::vec2(-1200, -600));
			TrackRef2->AddPoint(glm::vec2(0, 0));
			TrackRef2->MovePoint(0, glm::vec2(0));
			TrackRef2->MovePoint(1, glm::vec2(800, -2000));
			TrackRef2->MovePoint(3, glm::vec2(2300, 1000));
			TrackRef2->MovePoint(5, glm::vec2(-800, 2800));
			//TrackRef->MovePoint(7, glm::vec2(-900, -400));
		}

		int depth = 250;

		AINodeManager::GetSingleton().SetNodeDepth(depth);
		
		entt::entity TrackLimit = CreateEntity();
		SplineComponent* TrackLimitRef = &GetEntityRegister().emplace<SplineComponent>(TrackLimit);
		GetEntityRegister().emplace<WallTagLeft>(TrackLimit);
		TrackLimitRef->SetType(SplineType::Bezier);

		entt::entity TrackLimit2 = CreateEntity();
		SplineComponent* TrackLimitRef2 = &GetEntityRegister().emplace<SplineComponent>(TrackLimit2);
		GetEntityRegister().emplace<WallTagRight>(TrackLimit2);
		TrackLimitRef2->SetType(SplineType::Bezier);

		float scaler = 0.3f;
		for (auto entity : GetEntityRegister().view<SplineComponent, SplineTag>())
		{
			SplineComponent* TrackRefcomp = &GetEntityRegister().get<SplineComponent>(entity);

			glm::vec2* points = TrackRefcomp->GetSplineInPoints(depth);
			
			for (int i = 0; i < depth; i++)
			{
								
				b2Vec2 pos1;
				pos1.x = points[i].x;
				pos1.y = points[i].y;

				if(pos1.x == 0 && pos1.y == 0)
				{
					pos1.x = points[depth].x;
					pos1.y = points[depth].y;
				}
				
				int temp = (i == depth ? 0 : i + 1);				
				
				b2Vec2 pos2;
				pos2.x = points[temp].x;
				pos2.y = points[temp].y;

				//set the nodes
				glm::vec2 dir = glm::vec2(glm::vec2((pos2.x - pos1.x), (pos2.y - pos1.y)));
				dir = glm::normalize(dir);
				float tdepth = 1.f / static_cast<float>(depth);
				float posline = tdepth * i;
				AINodeManager::GetSingleton().GenerateNodes(points[i].x, points[i].y, dir.x, dir.y, posline);
				//end setting nodes

				glm::vec2 distBetweenTwoPoints = glm::vec2((pos2.x - pos1.x), (pos2.y - pos1.y));
				float AB = distBetweenTwoPoints.length();
				float BC = 30.0f;
				float AC = glm::sqrt((AB * AB) + (BC * BC));

				glm::vec2 WallPoint = glm::vec2(0,0);
							
				b2Vec2 WallPointb2left = Calulatecurve(pos1, pos2, false);
				b2Vec2 WallPointb2right = Calulatecurve(pos1, pos2, true);

				int indextemp = i - 1;
				if(indextemp < 0)
				{
					indextemp = depth -1;
				}
				b2Vec2 pos1prev;
				pos1prev.x = points[indextemp].x;
				pos1prev.y = points[indextemp].y;

				int indextemp2 = i ;
				if (indextemp2 > depth)
				{
					indextemp2 = 2;
				}
				b2Vec2 pos2prev;
				pos2prev.x = points[indextemp2].x;
				pos2prev.y = points[indextemp2].y;
				
				b2Vec2 WallPointb2leftprev = Calulatecurve(pos1prev, pos2prev, false);
				b2Vec2 WallPointb2rightprev = Calulatecurve(pos1prev, pos2prev, true);

				b2EdgeShape edgeshape;				
				b2BodyDef bd;
				b2FixtureDef fdf;
				bd.type = b2_staticBody;
				edgeshape.SetTwoSided(WallPointb2leftprev, WallPointb2left);
				PrevWallPointLeft = WallPointb2left;
				fdf.shape = &edgeshape;
				fdf.density = 1;
				fdf.filter.categoryBits = PhysicsComponent::CollMasks::Wall;
				fdf.filter.maskBits = PhysicsComponent::CollMasks::Wall;
				b2Body* bodyA = PhysicsWorld.CreateBody(&bd);
				bodyA->CreateFixture(&fdf);

				b2EdgeShape edgeshape2;				
				b2BodyDef bd2;
				b2FixtureDef fdf2;
				bd2.type = b2_staticBody;
				edgeshape2.SetTwoSided(WallPointb2rightprev, WallPointb2right);
				fdf2.shape = &edgeshape2;
				fdf2.density = 1;
				fdf2.filter.categoryBits = PhysicsComponent::CollMasks::General;
				fdf2.filter.maskBits = PhysicsComponent::CollMasks::Wall;
				b2Body* bodyB = PhysicsWorld.CreateBody(&bd2);
				bodyB->CreateFixture(&fdf2);
				
				for (auto entity : GetEntityRegister().view<SplineComponent, WallTagLeft>())
				{
					SplineComponent* TrackRefcompLeft = &GetEntityRegister().get<SplineComponent>(entity);
					TrackRefcompLeft->AddPoint(glm::vec2(WallPointb2right.x, WallPointb2right.y));
				}
				for (auto entity : GetEntityRegister().view<SplineComponent, WallTagRight>())
				{
					SplineComponent* TrackRefcompRight = &GetEntityRegister().get<SplineComponent>(entity);
					TrackRefcompRight->AddPoint(glm::vec2(WallPointb2left.x, WallPointb2left.y));
				}
			}
		}		
	}

	b2Vec2 WorldManager::Calulatecurve(const b2Vec2 pos1, const b2Vec2 pos2, bool external)
	{
		float w = pos2.x - pos1.x;
		float h = pos2.y - pos1.y;

		if (w == 0 || h == 0) {
			w = pos2.x - pos1.x;
			h = pos2.y - pos1.y;
		}

		float angle = glm::atan(h, w);

		if (external) {
			angle += HALF_PI;
		}
		else {
			angle -= HALF_PI;
		}

		float cx = (glm::cos(angle) * 30);
		float cy = (glm::sin(angle) * 30);

		b2Vec2 position;
		position.x = pos1.x + cx;
		position.y = pos1.y + cy;
		return position;
	}


	void WorldManager::Render(int a_camera)
	{

		// Rendering system.
		m_Renderer->Render(a_camera);
	}

	
	void WorldManager::LoadTrack()
	{

	/*	std::string name = "Track1.cereal";
		
		entt::entity Track = CreateEntity();
		SplineComponent* TrackRef = &GetEntityRegister().emplace<SplineComponent>(Track);
		GetEntityRegister().emplace<SplineTag>(Track);
		TrackRef->SetType(SplineType::Bezier);
		
		std::ofstream os(name.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive archive(os);

		
		for (int i = 0; i < 6; i++)
		{
			FileLoader::GetSingleton().Load(archive, *TrackRef);
		}*/
	
	}

	void WorldManager::SafeTrack()
	{
		
	}

	void WorldManager::SwitchCamera(int state)
	{		
			switch (state)
			{
			case 0:
			{
					
				Render(0);
				for (auto entity : GetEntityRegister().view<TransformComponent, SpriteComponent>())
				{
					if (!GetEntityRegister().has<StartScreen>(entity))
					{
						if (GetEntityRegister().has<SpriteComponent>(entity))
						{
							GetEntityRegister().remove<SpriteComponent>(entity);
						}
					}
				}
					
				for (auto entity : GetEntityRegister().view<SpriteTag, SIN::OrthoCameraComponent, TransformComponent, StartScreen>())
				{
					if(!GetEntityRegister().has<SpriteComponent>(entity))
					{
						SpriteTag* sptag = &GetEntityRegister().get<SpriteTag>(entity);
						GetEntityRegister().emplace<SpriteComponent>(entity, sptag->GetString(), glm::vec2(0,5));
					}					
				}			
			}
			break;
			case 1:
			{
				Render(1);
				for (auto entity : GetEntityRegister().view<TransformComponent, StartScreen, SpriteComponent>())
				{
					if (GetEntityRegister().has<SpriteComponent>(entity))
					{
						GetEntityRegister().remove<SpriteComponent>(entity);
					}
				}

				for (auto entity : GetEntityRegister().view<SpriteTag, TransformComponent>())
				{
					if(!GetEntityRegister().has<StartScreen>(entity))
					{
						if (!GetEntityRegister().has<SpriteComponent>(entity))
						{
							SpriteTag* sptag = &GetEntityRegister().get<SpriteTag>(entity);
							GetEntityRegister().emplace<SpriteComponent>(entity, sptag->GetString(), glm::vec2(0, 5));
						}
					}
					
				}
			}
			break;
			case 2:
			{
				Render(0);
				for (auto entity : GetEntityRegister().view<TransformComponent, SpriteComponent>())
				{
					if (!GetEntityRegister().has<StartScreen>(entity))
					{
						if (GetEntityRegister().has<SpriteComponent>(entity))
						{
							GetEntityRegister().remove<SpriteComponent>(entity);
						}
					}
					
				}

				for (auto entity : GetEntityRegister().view<SpriteTag, TransformComponent, StartScreen>())
				{
					if (!GetEntityRegister().has<SpriteComponent>(entity))
					{
						SpriteTag* sptag = &GetEntityRegister().get<SpriteTag>(entity);
						GetEntityRegister().emplace<SpriteComponent>(entity, sptag->GetString(), glm::vec2(0, 5));
					}
				}
			}
			break;
			default: break;
			}		
	}
}