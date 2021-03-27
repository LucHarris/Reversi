#pragma once

#include <random>
#include "Resources.h"
#include "StateManager.h"
#include "Board.h"
#include "MusicEngine.h"

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
	ReversiBoard reversiGame;
	MusicEngine music;
	// sf volume range from 0.0f - 100.0f
	float masterVolume = 30.0f;

};

