#pragma once

#include "Resources.h"
#include "StateManager.h"

class ReversiSFML
{

public:
	ReversiSFML();
	void Init();
	void Run();
	void Render(float);

	// sets up text elements with common state
	void InitText(sf::Text& t);
	
	StateManager stateManager;
	sf::RenderWindow window;
	Resources resources;
	sf::Text debugLog;


	
};

