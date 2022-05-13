#pragma once
#include <vector>
#include <Core/Systems.h>
#include <string>

#include "ContactListener.h"
#include "Systems/AISystem.h" 
#include "Systems/SplineSystem.h"

namespace SIN
{

	class Renderer;
	class TireSystem;
	class WorldManager
	{
	
	private:

		entt::registry m_Register;
		std::vector< Systems* > m_Systems;
		AISystem* m_AISystem;
		Renderer* m_Renderer;
		SplineSystem* m_SplineSystem;
		TireSystem* m_TireSystem;

	public:

		WorldManager(const WorldManager&) = delete;
		~WorldManager();
		void Initialize();

		/// <summary>
		/// </summary>
		/// <returns>Returns the instance of the worldmanager.</returns>
		static WorldManager& GetSingleton();

		/// <summary>
		/// Used for access to Renderer::MakeLine().
		/// </summary>
		/// <returns>The renderer</returns>
		Renderer* GetRenderer();

		/// <summary>
		/// Updates all the registered systems.
		/// </summary>
		void Update(float);

		/// <summary>
		/// Add a system to the register.
		/// </summary>
		/// <param name="System*"></param>
		void RegisterSystem(Systems*);
		 
		/// <summary>
		/// </summary>
		/// <returns>entity register from the worldmanager containing all information about the components and entities in the world. 
		/// Entities and components can be fetched, created or removed from here.</returns>
		entt::registry& GetEntityRegister();

		entt::entity CreateEntity();

		/// <summary>
		/// Delete an entity from the entt register.
		/// </summary>
		void DeleteEntity(entt::entity a_Entity);

		/// <summary>
		/// Remove component of type from the entity if it can be found on it.
		/// </summary>
		template<typename  T>
		void RemoveComponent(entt::entity a_Entity);

		/// <summary>
		/// Remove all components of an entity.
		/// </summary>
		void RemoveAllComponents(entt::entity a_Entity);
		entt::entity GetWorldRoot();

		b2World PhysicsWorld = b2World(b2Vec2(0.0f, 0.0f));

		void SwitchCamera(int state);
		void Render(int a_camera);
		
	private:

		WorldManager();
		void GenerateTrack();

		void LoadTrack();
		void SafeTrack();
		SplineComponent m_Track;

		std::string m_StartScreenFileName = "Assets/Textures/Drift.png";

		b2Vec2 PrevWallPointLeft;
		b2Vec2 PrevWallPointRight;
		MyContactListener myContactListenerInstance;
		b2Vec2 Calulatecurve(const b2Vec2 pos1,const b2Vec2 pos2, bool external);
	};

	template<typename  T>
	void WorldManager::RemoveComponent(entt::entity a_Entity)
	{
		m_Register.remove_if_exists<T>(a_Entity);
	}
}


