#include <Events/EventMessenger.h>

namespace SIN
{
	EventMessenger& EventMessenger::GetSingleton()
	{
		static EventMessenger messenger{};
		return messenger;
	}

	void EventMessenger::RemoveMessenger(const std::string& key)
	{
		m_eventDispatchers.erase(key);
	}

	bool EventMessenger::CheckIfExists(const std::string& key)
	{
		return (m_eventDispatchers.find(key) != m_eventDispatchers.end()) || (m_queuedEventDispatcher.find(key) != m_queuedEventDispatcher.end());
	}
}

