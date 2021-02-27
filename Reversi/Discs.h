#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.h"
#include "Grid.h"
class ReversiSFML;
class Discs
{
public:
	enum class DiscState{EMPTY,WHITE,BLACK,SHADOW};
private:
	Grid<sf::Sprite,64> mDiscSprites;
	Grid<DiscState,64> mDiscStates;
	DiscState mState = DiscState::EMPTY;
	ReversiSFML* mpApp;

public:
	Discs(ReversiSFML* app);
	void Init();
	void Update(float dt);
	void Render(float dt);
	// checks if disc has changed and sets accordingly 
	void Set(int x, int y, const DiscState& ds);
	

};

