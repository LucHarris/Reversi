#include "StateManager.h"
#include <assert.h>
#include <SFML/System.hpp>
#include "MainMenuState.h"
#include "GameSampleState.h"
#include "SelectionState.h"
#include "ReversiSFML.h"
#include "Constants.h"
#include "ProfileState.h"
#include "NetworkState.h"

void StateManager::InitStates()
{
	mStates.at(gc::STATE_INDEX_MAIN_MENU) = std::make_unique<MainMenuState>(mpApp);
	mStates.at(gc::STATE_INDEX_GAME_SAMPLE) = std::make_unique<GameSampleState>(mpApp);
	mStates.at(gc::STATE_INDEX_PLAYER_SELECTION) = std::make_unique<SelectionState>(mpApp);
	mStates.at(gc::STATE_INDEX_PROFILE) = std::make_unique<ProfileState>(mpApp);
	mStates.at(gc::STATE_INDEX_NETWORK) = std::make_unique<NetworkState>(mpApp);

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
	mHelpBtn(app),
	mChatBtn(app),
	mStates(gc::NUM_STATES)
{
	assert(app);
}

void StateManager::Init()
{
	InitStates();
	
	// viewport background fill
	mBackground.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_BG));
	mBackground.setTextureRect({ 0,0,gc::VIEWPORT_WIDTH_U,gc::VIEWPORT_HEIGHT_U });

	// normal buttons
	const sf::Vector2f btnBlOrigin{ 0.0f,64.0f };

	mMenuButtons.at(BTN_MENU).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_MENU));
	mMenuButtons.at(BTN_MENU).setOrigin(btnBlOrigin);
	mMenuButtons.at(BTN_MENU).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BL] + sf::Vector2f{ 64.0f,0.0f });

	mMenuButtons.at(BTN_STATS).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_STATS));
	mMenuButtons.at(BTN_STATS).setOrigin(btnBlOrigin);
	mMenuButtons.at(BTN_STATS).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BL] + sf::Vector2f{ 128.0f,0.0f });

	mMenuButtons.at(BTN_AUDIO).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_AUDIO));
	mMenuButtons.at(BTN_AUDIO).setOrigin(btnBlOrigin);
	mMenuButtons.at(BTN_AUDIO).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BL] + sf::Vector2f{ 192.0f,0.0f });

	// help icon properties setup
	mHelpBtn.icon.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_ICON));
	mHelpBtn.icon.setPosition(gc::VIEWPORT_WIDTH_F - 64.0f, gc::VIEWPORT_HEIGHT_F - 64.0f);

	// help display sprite properties setup
	const sf::Texture& t = mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_1);
	mHelpBtn.show.setTexture(t);
	
	// centre of texture
	sf::Vector2u origin = t.getSize();
	origin.y /= 2;

	mHelpBtn.show.setOrigin((sf::Vector2f)origin);
	mHelpBtn.show.setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_CR]);

	// Chat
	mChatBtn.icon.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_ICON));
	mChatBtn.icon.setPosition(0, gc::VIEWPORT_HEIGHT_F - 64.0f);

	const sf::Texture& c = mpApp->resources.GetTextureAt(Resources::TEXTURE_HELP_1);

	origin.x = 0;
	origin.y = c.getSize().y / 2;

	mChatBtn.show.setTexture(c);
	mChatBtn.show.setOrigin((sf::Vector2f)origin);
	mChatBtn.show.setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_CL]);

}

void StateManager::Update(float dt)
{
	mStates.at(mActiveState)->Update(dt);
}

void StateManager::Render(float dt)
{
	mpApp->window.draw(mBackground);

	for (auto& btn : mMenuButtons)
	{
		mpApp->window.draw(btn);
	}

	mStates.at(mActiveState)->Render(dt);

	mHelpBtn.Render(dt);
	mChatBtn.Render(dt);
}

void StateManager::MouseInput(const sf::Vector2f& pos)
{
	mStates.at(mActiveState)->MouseInput(pos);

	mHelpBtn.MouseInput(pos);
	mChatBtn.MouseInput(pos);
	// chat messages/input appears when toggled
	mpApp->chat.display = mChatBtn.mDisplay;

	if (mMenuButtons.at(BTN_MENU).getGlobalBounds().contains(pos))
	{
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_MAIN_MENU, true);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
	}

	if (mMenuButtons.at(BTN_STATS).getGlobalBounds().contains(pos))
	{
		//todo
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_PROFILE,true);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
	}

	if (mMenuButtons.at(BTN_AUDIO).getGlobalBounds().contains(pos))
	{
		mpApp->masterVolume = gc::VOL_TOGGLE - mpApp->masterVolume;
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
		mpApp->music.SetMasterVolume(mpApp->masterVolume * gc::HALF);
	}

}

void StateManager::KeyInput(sf::Keyboard::Key key)
{
	
}

void StateManager::TextEntered(unsigned int key)
{
	mStates.at(mActiveState)->TextEntered(key);

	//todo add logic for chat to display and enter
	// mChatBtn.mDisplay = true;
}

void StateManager::ChangeState(size_t s, bool reset)
{
	if (s < mStates.size())
	{
		if (mActiveState != s)
		{
			mActiveState = s;
			if (reset)
			{
				mStates.at(mActiveState)->Reset();
			}
			mpApp->music.PlayNext();
		}
		
	}
	else
	{
		assert(false);
	}
}
