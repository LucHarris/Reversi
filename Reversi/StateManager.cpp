#include "StateManager.h"
#include <assert.h>
#include <SFML/System.hpp>
#include "MainMenuState.h"
#include "ReversiSFML.h"
#include "Constants.h"
void StateManager::InitStates()
{
	mStates[StateKey::MENU] = std::make_unique<MainMenuState>(mpApp);
	// first state 
	mActiveState = StateKey::MENU;


	for (auto& s : mStates)
	{
		s.second->Init();
	}
}

StateManager::StateManager(ReversiSFML* app)
	:
	mpApp(app)
{
	assert(app);
}

void StateManager::Init()
{
	InitStates();
	
	// viewport background fill
	mBackground.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_BG));
	mBackground.setTextureRect({ 0,0,gc::ViewportWidth,gc::ViewportHeight });

}

void StateManager::Update(float dt)
{
	mStates.at(mActiveState)->Update(dt);
}

void StateManager::Render(float dt)
{
	mpApp->window.draw(mBackground);

	mStates.at(mActiveState)->Render(dt);
}

void StateManager::MouseInput(const sf::Vector2i& pos)
{
	mStates.at(mActiveState)->MouseInput(pos);
}

void StateManager::KeyInput(sf::Keyboard::Key key)
{
}
