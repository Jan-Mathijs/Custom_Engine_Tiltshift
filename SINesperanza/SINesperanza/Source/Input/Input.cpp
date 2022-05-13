#include "Input/Input.h"

namespace SIN
{
	Input::Input()
	{
		m_Window = nullptr;
	}

	Input::~Input()
	{
		delete this;
	}

	Input& Input::GetSingleton()
	{
		return *s_Input;
	}
	
	void Input::SetWindow(const Window* a_Window)
	{
		m_Window = a_Window;
	}
}
