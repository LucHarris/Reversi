#include "StateManager.h"
#include <assert.h>
#include <SFML/System.hpp>


StateManager::StateManager(ReversiSFML* app)
	:
	mpApp(app)
{
	assert(app);
}

void StateManager::Init()
{

}

void StateManager::Update(float dt)
{
	mStates.at(mActiveState)->Update(dt);
}

void StateManager::Render(float dt)
{
	mStates.at(mActiveState)->Render(dt);
}

void StateManager::MouseInput(const sf::Vector2i& pos)
{
	
}

void StateManager::KeyInput(sf::Keyboard::Key key)
{
}

void StateManager::Input()
{
	mStates.at(mActiveState)->Input();
}
