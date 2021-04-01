#pragma once
#include "DeltaTimer.h"
#include "State.h"
#include "Discs.h"
#include "Player.h"
#include "Node.h"

class GameSampleState :    public State
{
	sf::Sprite mBoardSprite;
	Discs mDiscSprites;
	std::vector<Player> mPlayers;
	// positive values mean active
	int mActivePlayer = 0;
	DeltaTimer mAiTimer;
	sf::Text mEndText;

	// increments active player for next turn
	void IncActivePlayer();
	// sets active player to invalid for update and input logic. sets end text
	void GameEnded();
	bool IsActiveGame();

public:
	GameSampleState(ReversiSFML* app);
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;
	virtual void MouseInput(const sf::Vector2f& pos) override;
	virtual void KeyInput(sf::Keyboard::Key key) override;
	virtual void TextEntered(unsigned int key) override {};
};

