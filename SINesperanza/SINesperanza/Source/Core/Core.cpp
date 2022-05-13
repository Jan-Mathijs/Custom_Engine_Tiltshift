#include <Core/Core.h>
#include <Core/WorldManager.h>
#include "Core/AINodeManager.h"
#ifdef PLATFORM_WIN64
#include "Graphics/Window.h"
#endif
#include <chrono>
#pragma warning(push)
#pragma warning(disable : 26812 26495)
#include "Box2d/b2_world.h"
#pragma warning(pop)
#include "Input/Input.h"
namespace SIN
{
	void Core::Initialize()
	{
#ifdef PLATFORM_WIN64
		m_Window = new Window(720, 480, "Hello owl!"); //These parameters are completely arbitrary and should be replaced with variables
#endif
		WorldManager::GetSingleton().Initialize();
	}
	
	
	void Core::MainLoop()
	{
		//double dt{};
		std::chrono::high_resolution_clock timer;
		std::chrono::high_resolution_clock::time_point start = timer.now(), stop;

		auto currenttime = std::chrono::steady_clock::now();
		float totaltime = 0.0f;
		unsigned int frames = 0;
		//std::chrono::duration<float,std::milli> dt = (m_time_point2 - m_time_point1);
		
		NextState();
		//float lastTick = getMilliseconds();
		float dt{};
		while (m_Running)
		{
			stop = timer.now();
			std::chrono::duration<double> elapsed = stop - start;

			dt = static_cast<float>(elapsed.count());
			
			if (INPUT.IsKeyPressed(SIN_KEY_ESCAPE))
			{
				AINodeManager::GetSingleton().SafeNodes();
				m_Running = false;
			}

			if (INPUT.IsKeyPressed(SIN_KEY_ENTER))
			{
				NextState();				
			}

			switch (m_CurrentState)
			{
			case GameState::StartScreen:
				{
					WorldManager::GetSingleton().Render(0);
				}
				break;
			case GameState::GamePlay:
				{
				
					WorldManager::GetSingleton().Update(dt);				
				}
				break;
			case GameState::EndScreen:
				{
					WorldManager::GetSingleton().Render(0);
				}
				break;
			default: break;;
			}
			
			WorldManager::GetSingleton().PhysicsWorld.Step(dt, 16, 6);
			m_Window->Update();
			if (m_Window->ShouldClose())
			{
				m_Running = false;
			}
			m_timer += dt;
			start = stop;
		}
	}

	void Core::ShutDown()
	{
		m_Running = false;
	}

	void Core::NextState()
	{
		if (m_timer <= m_minTime) return;
		
		switch (m_CurrentState)
		{
		case GameState::StartScreen:
			{
				m_CurrentState = GameState::GamePlay;

				WorldManager::GetSingleton().SwitchCamera(1);
				m_timer = 0;
			}
			break;
		case GameState::GamePlay:
			{
				m_CurrentState = GameState::EndScreen;
				WorldManager::GetSingleton().SwitchCamera(2);
				m_timer = 0;
			}
			break;
		case GameState::EndScreen:
			{
				m_CurrentState = GameState::StartScreen;
				WorldManager::GetSingleton().SwitchCamera(0);
				m_timer = 0;
			}
			break;
		default: break;
		}		
	}

	
}
