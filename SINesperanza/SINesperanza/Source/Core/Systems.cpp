#include <Core/Systems.h>
#include <Core/WorldManager.h>

namespace  SIN
{
	void Systems::Register()
	{
		WorldManager::GetSingleton().RegisterSystem(this);
	}

	entt::registry& Systems::GetEntityRegister()
	{
		return WorldManager::GetSingleton().GetEntityRegister();
	}

	void Systems::DeleteEntity(entt::entity a_Entity)
	{
		WorldManager::GetSingleton().DeleteEntity(a_Entity);
	}

	void Systems::RemoveAllComponents(entt::entity a_Entity)
	{
		GetEntityRegister().remove_all(a_Entity);
	}

	void Systems::Reserve(int a_Size)
	{
		GetEntityRegister().reserve(a_Size);
	}

	size_t Systems::GetEntityCount()
	{
		return GetEntityRegister().size();
	}

	size_t Systems::GetEntityCapacity()
	{
		return GetEntityRegister().capacity();
	}

	entt::entity Systems::CreateEntity()
	{
		return WorldManager::GetSingleton().CreateEntity();
	}
}