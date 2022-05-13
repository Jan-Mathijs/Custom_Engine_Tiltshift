#pragma once
#include <iostream>

#include "Events/EventArgs.h"
#include "Core/Systems.h"


class InputTestSystem :public SIN::Systems
{

public:

	InputTestSystem();
	~InputTestSystem();

	virtual void OnUpdate(float a_DeltaTime);

private:

	void MouseMovement(SIN::MouseMotionEventArgs& a_Args);
	void InputPress(SIN::KeyEventArgs& args);
};

