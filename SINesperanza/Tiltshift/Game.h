#pragma once
#include <Core/EntryPoint.h>
//#include <Diabolo/Diabolo.h>

class Game : public SIN::Core
{
public:
	Game() : SIN::Core() {}
	~Game() = default;

	void Initialize();
	void Shutdown();
	
};
