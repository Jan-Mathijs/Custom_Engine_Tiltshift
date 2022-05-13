#pragma once
#include <Core/Core.h>

extern SIN::Core* SIN::CreateGame();

int main()
{
	auto app = SIN::CreateGame();
	app->Initialize();
	delete app;

	return  0;
}