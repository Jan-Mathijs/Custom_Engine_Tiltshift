
#include <Events/InputHandler.h>
#ifdef PLATFORM_WIN64
#include <Input/WinController.h>
typedef SIN::WinController Controller;
#endif


namespace SIN
{
	InputHandler& InputHandler::GetSingleton()
	{
		static InputHandler instance;
		return instance;
	}

	void InputHandler::ShutDown()
	{
		for (int i = 0; i < m_controllers.size(); ++i)
		{
			delete m_controllers[i].second;
			m_controllers[i].second = nullptr;
		}
	}

	void InputHandler::UpdateInput()
	{
		
		std::vector<const ControllerState*> controllerStates;
		controllerStates.resize(m_controllers.size());
		// Poll all the controllers and get their state
		for (int i = 0; i < m_controllers.size(); ++i)
		{
			m_controllers[i].second->UpdateStatus();
			controllerStates[i] = &m_controllers[i].second->GetState();
		}
		// Evoke the controller event
		m_ControllerStateEvent(controllerStates);
	}

	void InputHandler::InvokeKeyPressedEvent(KeyEventArgs& args)
	{
		m_keyPressedEvent(args);
	}

	void InputHandler::InvokeKeyReleasedEvent(KeyEventArgs& args)
	{
		m_keyReleasedEvent(args);
	}

	void InputHandler::InvokeMouseMotionEvent(MouseMotionEventArgs& args)
	{
		m_mouseMotionEvent(args);
	}

	void InputHandler::InvokeMouseButtonPressedEvent(MouseButtonEventArgs& args)
	{
		m_mouseButtonPressedEvent(args);
	}

	void InputHandler::InvokeMouseButtonReleasedEvent(MouseButtonEventArgs& args)
	{
		m_mouseButtonReleasedEvent(args);
	}

	void InputHandler::InvokeMouseWheelEvent(MouseWheelEventArgs& args)
	{
		m_mouseWheelEvent(args);
	}

	KeyEvent& InputHandler::GetKeyPresedEventDelegate()
	{
		return m_keyPressedEvent;
	}

	KeyEvent& InputHandler::GetKeyReleasedEventDelegate()
	{
		return m_keyReleasedEvent;
	}

	MouseMotionEvent& InputHandler::GetMouseMotionDelegate()
	{
		return m_mouseMotionEvent;
	}

	MouseButtonEvent& InputHandler::GetMouseButtonPressedDelegate()
	{
		return m_mouseButtonPressedEvent;
	}

	MouseButtonEvent& InputHandler::GetMouseButtonReleasedDelegate()
	{
		return m_mouseButtonReleasedEvent;
	}

	MouseWheelEvent& InputHandler::GetMouseWheelDelegate()
	{
		return m_mouseWheelEvent;
	}

	ControllerStateEvent& InputHandler::GetControllerStateDelegate()
	{
		return 	m_ControllerStateEvent;
	}

	int InputHandler::AddController()
	{
		IController* ptr = new Controller{};
		ptr->InitializeController();
		m_controllers.push_back(std::make_pair(m_controllerID++,ptr));
		return m_controllerID - 1;
	}

	void InputHandler::RemoveController(int id)
	{
		bool removedElement{ false };
		for (int i = 0; i < m_controllers.size(); ++i)
		{
			if(m_controllers[i].first == id)
			{
				removedElement = true;
				m_controllers.erase(m_controllers.begin() + i);
				break;
			}
		}
		assert(removedElement);
	}
}
