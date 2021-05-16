#pragma once
#include "State.h"
#include "SFML/Graphics.hpp"
#include <array>
#include <string>
#include "OutputJobs.h"
#include "DeltaTimer.h"

// players/cpu selected for a game
class SelectionState : public State
{
	// two lists of players
	std::array<sf::Text, 2> mPlayerDisplay;
	std::array<sf::Sprite, 9> mButtons;
	sf::Text mText;
	sf::Sprite mBlackboard;
	DeltaCountdownTimer mRefresh;

	enum{
		BTN_WHITE_PLAYER_PLUS = 0,
		BTN_WHITE_REMOVE_ONE,
		BTN_WHITE_CPU_PLUS,
		BTN_WHITE_REMOVE_ALL,
		BTN_BLACK_PLAYER_PLUS,
		BTN_BLACK_REMOVE_ONE,
		BTN_BLACK_CPU_PLUS,
		BTN_BLACK_REMOVE_ALL,
		BTN_PLAY,
		BTN_SIZE
	};
public:
	SelectionState(ReversiSFML* app);
	virtual void Init() override;
	virtual void Update(float)	override;
	virtual void Render(float) override;
	virtual void MouseInput(const sf::Vector2f&) override;
	virtual void KeyInput(sf::Keyboard::Key) override;
	virtual void TextEntered(unsigned int key) override;
	virtual void Reset() override;

	void UpdatePlayerList();
private:
	void ClientSendOperation(ClientSendData::ButtonOp op);
};

