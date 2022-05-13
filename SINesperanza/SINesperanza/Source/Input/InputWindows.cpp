#include "Input/InputWindows.h"
#include "Graphics/Window.h"
#include  <iostream>
namespace SIN
{
	// Callback methods.
	void CallbackScroll(GLFWwindow* a_window, double a_X, double a_Y)
	{
		dynamic_cast<InputWindows&>(Input::GetSingleton()).SetScroll(static_cast<float>(a_Y));
	}

	InputWindows::InputWindows()
	{
		m_Scroll = 0;
	}

	void InputWindows::Clear()
	{
		m_Scroll = 0;
	}

	
	void InputWindows::Initialize()
	{
		glfwSetScrollCallback(m_Window->GetWindow(), CallbackScroll);
	}

	bool InputWindows::ImplIsKeyPressed(const int a_Keycode) const
	{
		int state = glfwGetKey(m_Window->GetWindow(), a_Keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool InputWindows::ImplMouseButtonPressed(const int a_MouseButton) const
	{
		int state = glfwGetMouseButton(m_Window->GetWindow(), a_MouseButton);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> InputWindows::ImplGetMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(m_Window->GetWindow(), &x, &y);
		return {static_cast<float>(x), static_cast<float>(y) };
	}

	float InputWindows::ImplGetScroll() const
	{
		return m_Scroll;
	}

	void InputWindows::SetScroll(float a_Scroll)
	{
		m_Scroll = a_Scroll;
	}

	bool InputWindows::ImplGetControllerButton(int a_Button) const
	{
		GLFWgamepadstate gstate;
		const int state = glfwGetGamepadState(GLFW_JOYSTICK_1, &gstate);

		if(state)
		{
			const int isPressed = gstate.buttons[a_Button] == GLFW_PRESS;
			return isPressed;
		}

		return false;
	}

	float InputWindows::ImplGetTriggerAxis(int a_Trigger) const
	{
		GLFWgamepadstate gstate;
		const int state = glfwGetGamepadState(GLFW_JOYSTICK_1, &gstate);

		if (state)
		{
			return gstate.axes[a_Trigger];
		}

		return 0;
	}

	std::pair<float, float> InputWindows::ImplGetStickAxis(int a_Trigger) const
	{
		GLFWgamepadstate gstate;
		const int state = glfwGetGamepadState(GLFW_JOYSTICK_1, &gstate);
		float x, y;
		
		if (state)
		{
			if(a_Trigger == GLFW_GAMEPAD_AXIS_LEFT_X || a_Trigger == GLFW_GAMEPAD_AXIS_LEFT_Y)
			{
				x = gstate.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
				y = gstate.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
			}
			else if (a_Trigger == GLFW_GAMEPAD_AXIS_RIGHT_X || a_Trigger == GLFW_GAMEPAD_AXIS_RIGHT_Y)
			{
				x = gstate.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
				y = gstate.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
			}
			else
			{
				x = y = 0;
			}
		}

		return {x,y};
	}


}
