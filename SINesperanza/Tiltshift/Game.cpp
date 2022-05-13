#include "Game.h"
#include <iostream>
#include <Components/TransformComponent.h>
#include <entt/entt.hpp>
//#include "InputTestSystem.h"
#include "Components/AIComponent.h"
#include "Components/SplineComponent.h"
#include "Core/Systems.h"
#include "AICarSystem.h"
#include "PlayerCar.h"
void Game::Initialize()
{
	//Initialize the core.
	Core::Initialize();
	
	std::cout << "Initialize Game" << std::endl;
	// Add everything related to the game.

	//InputTestSystem inputTest;
	AICarSystem carSystemAI;
	PlayerCarSystem playercar;
	
	Core::MainLoop();

}

void Game::Shutdown()
{

}

SIN::Core* SIN::CreateGame()
{
	return new Game();
}