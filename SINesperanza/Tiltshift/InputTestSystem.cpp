#include "InputTestSystem.h"
#include "Events/InputHandler.h"
#include "Events/EventMessenger.h"
//#include <GLFW/glfw3.h>
#include "Input/Input.h"

InputTestSystem::InputTestSystem()
{
	Register();

	std::cout << "initialize test system" << std::endl;

	SIN::InputHandler::GetSingleton().GetKeyReleasedEventDelegate().Connect(&InputTestSystem::InputPress, this);
	SIN::InputHandler::GetSingleton().GetMouseMotionDelegate().Connect(&InputTestSystem::MouseMovement, this);
}

InputTestSystem::~InputTestSystem()
{

}

void InputTestSystem::OnUpdate(float a_DeltaTime)
{
	//std::cout << SIN::Input::IsKeyPressed(GLFW_KEY_W) << std::endl;
	//std::cout << SIN::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) << std::endl;
	//std::cout << SIN::Input::GetMousePosition().first << std::endl;
	//std::cout << SIN::Input::GetScroll() << std::endl;

	//std::cout << SIN::Input::GetControllerButton(GLFW_GAMEPAD_BUTTON_A) << std::endl;
	//std::cout << SIN::Input::GetTriggerAxis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) << std::endl;
	//std::cout << SIN::Input::GetStickAxis(GLFW_GAMEPAD_AXIS_RIGHT_X).first << std::endl;

	//std::cout << INPUT.IsKeyPressed(GLFW_KEY_W) << std::endl;

	if(INPUT.IsKeyPressed(SIN_KEY_W))
	{
		std::cout << "W Pressed" << std::endl;
	}


	/*if(SIN::Input::IsKeyPressed(GLFW_KEY_W))
	{
		
	}*/
	
}

void InputTestSystem::MouseMovement(SIN::MouseMotionEventArgs& args)
{
	std::cout << "Mouse Movement" << std::endl;
}

void InputTestSystem::InputPress(SIN::KeyEventArgs& args)
{
	std::cout << "Keyboard press" << std::endl;
}