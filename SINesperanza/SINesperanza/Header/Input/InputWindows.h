#pragma once
#include "Input.h"
#include "GLFW/glfw3.h"
namespace SIN
{
	class InputWindows : public Input
	{
	private:

		float m_Scroll;

	public:

		InputWindows();
		
		void Initialize() override;
		void Clear() override;

		void SetScroll(float a_Scroll);
	
	protected:

		[[nodiscard]] bool ImplIsKeyPressed(int a_Keycode) const override;
		[[nodiscard]] bool ImplMouseButtonPressed(int a_MouseButton) const override;
		[[nodiscard]] std::pair<float, float> ImplGetMousePosition() const override;
		[[nodiscard]] float ImplGetScroll() const override;
		[[nodiscard]] bool ImplGetControllerButton(int a_Button) const override;
		[[nodiscard]] float ImplGetTriggerAxis(int a_Trigger) const override;
		[[nodiscard]] std::pair<float, float> ImplGetStickAxis(int a_Trigger) const override;
	};
}