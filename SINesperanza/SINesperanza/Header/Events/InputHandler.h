#pragma once
#include<Events/Delegate.h>
#include<Events/EventArgs.h>
#include <Input/IController.h>

#include <vector>
#include <utility>
namespace SIN 
{
	
	using KeyEvent = Delegate<KeyEventArgs&>;
	using MouseMotionEvent = Delegate<MouseMotionEventArgs&>;
	using MouseButtonEvent = Delegate<MouseButtonEventArgs&>;
	using MouseWheelEvent = Delegate<MouseWheelEventArgs&>;
	using ControllerStateEvent = Delegate<const std::vector<const ControllerState*>&>;

	class InputHandler
	{
	public:

	
		InputHandler() = default;
		~InputHandler() = default;
		static InputHandler& GetSingleton();
		void ShutDown();

		/**
		 * \brief needs to be called each frame to poll certain events. such as controller state
		 */
		void UpdateInput();
		
		/**
		 * \brief Invoke the delegate for the key pressed event calling all subscribed functions
		 * \param args to the event.
		 */
		void InvokeKeyPressedEvent(KeyEventArgs& args);
		/**
		 * \brief Invoke the delegate for the key released event calling all subscribed functions
		 * \param args to the event
		 */
		void InvokeKeyReleasedEvent(KeyEventArgs& args);
		/**
		 * \brief Invoke the delegate for the mouse button pressed event calling all subscribed functions
		 * \param args to the event
		 */
		void InvokeMouseButtonPressedEvent(MouseButtonEventArgs& args);
		/**
		 * \brief Invoke the delegate for the mouse button released event calling all subscribed functions
		 * \param args to the event
		 */
		void InvokeMouseButtonReleasedEvent(MouseButtonEventArgs& args);
		/**
		 * \brief Invoke the delegate for the mouse motion event calling all subscribed functions
		 * \param args to the event
		 */
		void InvokeMouseMotionEvent(MouseMotionEventArgs& args);
		/**
		 * \brief Invoke the delegate for the mouse wheel event calling all subscribed functions
		 * \param args to the event
		 */
		void InvokeMouseWheelEvent(MouseWheelEventArgs& args);
		
		/**
		 * \brief Get the underlying delegate for the key pressed event
		 * \return the key pressed event delegate
		 */
		KeyEvent& GetKeyPresedEventDelegate();
		/**
		 * \brief Get the underlying delegate for the key released event
		 * \return the key released event delegate
		 */
		KeyEvent& GetKeyReleasedEventDelegate();
		/**
		 * \brief Get the underlying delegate for the mouse button pressed event
		 * \return the mouse button pressed event delegate
		 */
		MouseButtonEvent& GetMouseButtonPressedDelegate();
		/**
		 * \brief Get the underlying delegate for the mouse button released event
		 * \return the mouse button released event delegate
		 */
		MouseButtonEvent& GetMouseButtonReleasedDelegate();
		/**
		 * \brief Get the underlying delegate for the mouse motion event
		 * \return the mouse motion event delegate
		 */
		MouseMotionEvent& GetMouseMotionDelegate();
		/**
		 * \brief Get the underlying delegate for the mouse wheel event
		 * \return the mouse wheel event delegate
		 */
		MouseWheelEvent& GetMouseWheelDelegate();
		/**
		 * \brief Get the underlying delegate for the controller state event
		 * \return return the controller state delegate
		 */
		ControllerStateEvent& GetControllerStateDelegate();


		/**
		 * \brief Add a new Controller
		 * \return The controller ID
		 */
		int AddController();
		/**
		 * \brief Remove a specific controller
		 * \param id The controller ID
		 */
		void RemoveController(int id);
	protected:



		KeyEvent m_keyPressedEvent{};
		KeyEvent m_keyReleasedEvent{};
		MouseButtonEvent m_mouseButtonPressedEvent{};
		MouseButtonEvent m_mouseButtonReleasedEvent{};
		MouseMotionEvent m_mouseMotionEvent{};
		MouseWheelEvent m_mouseWheelEvent{};
		ControllerStateEvent m_ControllerStateEvent{};

	private:
		std::vector<std::pair<int, IController* >> m_controllers;
		int m_controllerID{};
	};


}
