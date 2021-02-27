#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.h"
#include "Grid.h"
class ReversiSFML;


class Discs
{
public:
	enum class DiscState{EMPTY,WHITE,BLACK,SHADOW};
	struct DiscSprite
	{
		sf::Sprite sprite;
		Discs::DiscState state = Discs::DiscState::EMPTY;
	};
private:
	
	Grid<DiscSprite, 8> mSprites;
	DiscState mState = DiscState::EMPTY;
	ReversiSFML* mpApp;

public:
	Discs(ReversiSFML* app);
	void Init();
	void Update(float dt);
	void Render(float dt);
	// checks if disc has changed and sets accordingly 
	void Set(int a, const DiscState& ds);

	void MouseInput(const sf::Vector2f& pos);
	void UpdateDiscs();

};

