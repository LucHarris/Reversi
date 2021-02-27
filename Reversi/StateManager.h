#pragma once

#include "State.h"
#include <map>
#include <memory>


enum class StateKey : size_t {MENU,GAME_SAMPLE};

typedef std::vector<std::unique_ptr<State>> StateContainer;

class StateManager
{
	StateContainer mStates;
	sf::Sprite mBackground;
	ReversiSFML* mpApp;
	size_t mActiveState;

	void InitStates();
public:
	StateManager(ReversiSFML* app);
	void Init();
	void Update(float dt);
	void Render(float dt);
	void MouseInput(const sf::Vector2i& pos);
	void KeyInput(sf::Keyboard::Key key);
	// Immediately changes active state
	void ChangeState(size_t s);

};

