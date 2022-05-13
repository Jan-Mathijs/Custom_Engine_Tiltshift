#pragma once
#include <Input/ControllerState.h>

namespace SIN
{
	class IController
	{
	public:
		IController() = default;
		virtual ~IController() = default;
		virtual void InitializeController() = 0;
		virtual void UpdateStatus() = 0;

		const ControllerState& GetState();

	protected:
		ControllerState m_controllerState{};
	};
}