#pragma once

#include <map>
#include <memory>
#include "State.h"
#include "ToggleButton.h"
#include "Constants.h"

enum class StateKey : size_t {MENU,GAME_SAMPLE};


typedef std::vector<std::unique_ptr<State>> StateContainer;

class StateManager
{
	StateContainer mStates;
	sf::Sprite mBackground;
	ToggleButton mHelp;
	ToggleButton mChat;
	ReversiSFML* mpApp;
	
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
	void ChangeState(size_t s);

};