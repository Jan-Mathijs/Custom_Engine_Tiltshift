#include "Graphics/Window.h"
#include <iostream>
#include "Input/InputWindows.h"

using namespace SIN;

Input* Input::s_Input = new InputWindows();

Window::Window(int width, int height, const char* windowName)
{
    if (!glfwInit())
        printf("Whoa, initialisiong GLFW went wrong!");

    m_Window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
    if (!m_Window)
    {
        printf("Whoa, creating a GLFW window went wrong!");
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);

    Input::GetSingleton().SetWindow(this);
    Input::GetSingleton().Initialize();
}

Window::~Window()
{	
    if (m_Window)
        glfwTerminate();
}

void Window::Update()
{
    Input::GetSingleton().Clear();
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

GLFWwindow* SIN::Window::GetWindow() const
{
    return m_Window;
}
