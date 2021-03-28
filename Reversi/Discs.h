#pragma once
#include "SFML/Graphics.hpp"
#include "Constants.h"
#include "Grid.h"
class ReversiSFML;

// grid of sprites. Process inputs to moves 
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
	sf::Sprite mCursor;
public:
	Discs(ReversiSFML* app);
	void Init();
	void Update(float dt);
	void Render(float dt);
	// checks if disc has changed and sets accordingly 
	void Set(int a, const DiscState& ds);

	void MoveByMousePos(const sf::Vector2f& pos);

	// checks mouse position against tile global bounds
	// @param mouse position
	// @return successful move location if value is 0 or higher
	int MoveByMoooouse(const sf::Vector2f& pos);
	void UpdateDiscs();
	// 1d move location on board
	bool Move(int move);
};

