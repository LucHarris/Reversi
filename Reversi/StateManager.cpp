#include "StateManager.h"

void StateManager::Initialize()
{
}

void StateManager::Update(const GameTimer& gt)
{
	mStates.at(mActiveState)->Update(gt);
}

void StateManager::Render(const GameTimer& gt)
{
	mStates.at(mActiveState)->Render(gt);
}

void StateManager::Input()
{
	mStates.at(mActiveState)->Input();
}
