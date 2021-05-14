#pragma once
#include <array>
#include "DeltaTimer.h"
#include "State.h"
#include "Discs.h"
#include "Player.h"
#include "Node.h"
#include "PlayerManager.h"
#include "PayoffMultipliers.h"
#include "NormalForm.h"

class GameSampleState :    public State
{
	enum{TEXT_END,TEXT_TURN};
	sf::Sprite mBoardSprite;
	Discs mDiscSprites;
	PlayerManager* mpSelectionPlayers;
	// positive values mean active
	DeltaCountdownTimer mAiTimer;
	std::array<sf::Text,2> mText;
	PayoffMultipliers mPayoffMulti;
	// ai structure for each side 
	NormalForm mAiNormalForm[2];
	

public:
	GameSampleState(ReversiSFML* app);
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;
	virtual void MouseInput(const sf::Vector2f& pos) override;
	virtual void KeyInput(sf::Keyboard::Key key) override;
	virtual void TextEntered(unsigned int key) override {};

	virtual void Reset() override;
private:
	// increments active player for next turn
	void IncActivePlayer();
	// sets active player to invalid for update and input logic. sets end text
	void GameEnded();

	void UpdateTurnList();

};

