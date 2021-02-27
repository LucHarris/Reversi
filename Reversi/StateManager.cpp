#include "StateManager.h"
#include <assert.h>
#include <SFML/System.hpp>
#include "MainMenuState.h"
#include "GameSampleState.h"
#include "ReversiSFML.h"
#include "Constants.h"
void StateManager::InitStates()
{
	mStates.at(gc::STATE_INDEX_MAIN_MENU) = std::make_unique<MainMenuState>(mpApp);
	mStates.at(gc::STATE_INDEX_GAME_SAMPLE) = std::make_unique<GameSampleState>(mpApp);

	// first state 
	mActiveState = gc::STATE_INDEX_MAIN_MENU;

	for (std::unique_ptr<State>& s : mStates)
	{
		s->Init();
	}
}

StateManager::StateManager(ReversiSFML* app)
	:
	mpApp(app),
	mStates(2)
{
	assert(app);
}

void StateManager::Init()
{
	InitStates();
	
	// viewport background fill
	mBackground.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_BG));
	mBackground.setTextureRect({ 0,0,gc::VIEWPORT_WIDTH_U,gc::VIEWPORT_HEIGHT_U });

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

void StateManager::ChangeState(size_t s)
{
	if (s < mStates.size())
	{
		mActiveState = s;
	}
	else
	{
		assert(false);
	}
}
