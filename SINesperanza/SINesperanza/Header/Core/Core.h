#pragma once

namespace SIN 
{
	/// <summary>
	/// Represents the core of the engine, should be created on startup.
	/// </summary>
	
	class Window;
	class Core 
	{
	public:
		Core() = default;
		virtual ~Core() = default;

		virtual void Initialize();
		virtual void ShutDown();

		enum m_collMasks
		{
			General = 0x0001,
			Wall = 0x0002,
			Player = 0x0004,
			Cars = 0x0008,
		};
		
	protected:
		void MainLoop();
	
	private:
		bool m_Running = true;
		Window* m_Window;

		enum class GameState
		{
			StartScreen = 0,
			GamePlay = 1,
			EndScreen = 2,
		};

		void NextState();

		GameState m_CurrentState = GameState::EndScreen;

		float m_timer = 100;
		float m_minTime = 1.f;
	};

	Core* CreateGame();
	
}

