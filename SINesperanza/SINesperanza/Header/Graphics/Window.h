#pragma once
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace SIN {

	/// <summary>
	/// Stuff
	/// </summary>
	
	class Window
	{
	public:
		Window(int width, int height, const char* wndName);
		~Window();
		//Swaps backbuffers and polls events
		void Update();
		//Check if the window should close, use this to query GLFW if the window was manually closed
		bool ShouldClose();

		[[nodiscard]] GLFWwindow* GetWindow() const;
	private:
		GLFWwindow* m_Window; //Mind you, GLFWWindow is this wrapper class, GLFWwindow is the underlying GLFW-window class
	};

}