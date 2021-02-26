#pragma once

#include "Resources.h"
#include "StateManager.h"

class ReversiSFML
{
	StateManager mStateManager;


public:
	ReversiSFML();
	void Init();
	void Run();
	void Render(float);
	sf::RenderWindow window;
	Resources resources;


	
};

