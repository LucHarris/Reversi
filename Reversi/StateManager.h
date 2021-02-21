#pragma once

#include "State.h"
#include <map>
#include <memory>

enum class StateKey{MENU,ONE_PLAYER,TWO_PLAYER};

class StateManager
{
	
	std::map<StateKey,std::unique_ptr<State>> mStates;

	StateKey mActiveState;

public:
	void Initialize();
	void Update(const GameTimer& gt);
	void Render(const GameTimer& gt);
	void Input();

};

