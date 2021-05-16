#pragma once

#include <map>
#include <memory>
#include "State.h"
#include "ToggleButton.h"
#include "Constants.h"

enum class StateKey : size_t {MENU,GAME_SAMPLE,PLAYER_SELECTION};


typedef std::vector<std::unique_ptr<State>> StateContainer;

class StateManager
{
	StateContainer mStates;
	sf::Sprite mBackground;
	ToggleButton mHelpBtn;
	ToggleButton mChatBtn;
	ReversiSFML* mpApp;
	std::array<sf::Sprite,3> mMenuButtons;
	sf::Text mText;
	enum 
	{
		BTN_MENU,BTN_STATS,BTN_AUDIO
	};
public:
	// copy, rotate, input
	std::array<std::string, gc::CHAT_SIZE> chatMessages;
private:
	size_t mActiveState;

	void InitStates();
public:
	StateManager(ReversiSFML* app);
	void Init();
	void Update(float dt);
	void Render(float dt);
	void MouseInput(const sf::Vector2f& pos);
	void KeyInput(sf::Keyboard::Key key);
	void TextEntered(unsigned int key);
	// Immediately changes active state
	// only change/reset if the new state isn't the old one
	void ChangeState(size_t s, bool reset = false);

};