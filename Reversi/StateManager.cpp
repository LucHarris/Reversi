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
	mHelp(app),
	mChat(app),
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

	// help icon properties setup
	mHelp.icon.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_ICON));
	mHelp.icon.setPosition(gc::VIEWPORT_WIDTH_F - 64.0f, gc::VIEWPORT_HEIGHT_F - 64.0f);

	// help display sprite properties setup
	const sf::Texture& t = mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_1);
	mHelp.show.setTexture(t);
	
	// centre of texture
	sf::Vector2u origin = t.getSize();
	origin.y /= 2;

	mHelp.show.setOrigin((sf::Vector2f)origin);
	mHelp.show.setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_CR]);

	// Chat
	mChat.icon.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_ICON));
	mChat.icon.setPosition(0, gc::VIEWPORT_HEIGHT_F - 64.0f);

	const sf::Texture& c = mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_1);

	origin.x = 0;
	origin.y = c.getSize().y / 2;

	mChat.show.setTexture(c);
	mChat.show.setOrigin((sf::Vector2f)origin);
	mChat.show.setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_CL]);

}

void StateManager::Update(float dt)
{
	mStates.at(mActiveState)->Update(dt);
}

void StateManager::Render(float dt)
{
	mpApp->window.draw(mBackground);

	mStates.at(mActiveState)->Render(dt);

	mHelp.Render(dt);
	mChat.Render(dt);

}

void StateManager::MouseInput(const sf::Vector2f& pos)
{
	mStates.at(mActiveState)->MouseInput(pos);

	mHelp.MouseInput(pos);
	mChat.MouseInput(pos);
}

void StateManager::KeyInput(sf::Keyboard::Key key)
{
}

void StateManager::TextEntered(unsigned int key)
{
	mStates.at(mActiveState)->TextEntered(key);

	// show existing chat
	mChat.mDisplay = true;
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
