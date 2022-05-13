#include <Input/IController.h>

namespace SIN
{
	const ControllerState& IController::GetState()
	{
		return m_controllerState;
	}
}

