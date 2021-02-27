#pragma once

#include "State.h"
#include <map>
#include <memory>

enum class StateKey{MENU,ONE_PLAYER,TWO_PLAYER};

typedef std::map<StateKey, std::unique_ptr<State>> StateMap;

class StateManager
{
	StateMap mStates;
	sf::Sprite mBackground;
	ReversiSFML* mpApp;
	StateKey mActiveState = StateKey::MENU;

	void InitStates();
public:
	StateManager(ReversiSFML* app);
	void Init();
	void Update(float dt);
	void Render(float dt);
	void MouseInput(const sf::Vector2i& pos);
	void KeyInput(sf::Keyboard::Key key);

};

