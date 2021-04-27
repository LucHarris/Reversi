#include "MainMenuState.h"
#include <assert.h>
#include "ReversiSFML.h"
#include "Constants.h"

MainMenuState::MainMenuState(ReversiSFML* app)
	:
	State(app),
	mButtons{app,app,app}
{
}

void MainMenuState::Init()
{
	float logoRelPosY = 0.17f;

	mLogoStretch.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_STRETCH));
	mLogoStretch.setOrigin(((sf::Vector2f)(mpApp->resources.GetTextureAt(Resources::TEXTURE_STRETCH).getSize())) * 0.5f);
	mLogoStretch.setTextureRect({ 0,0,gc::VIEWPORT_WIDTH_U,128 });

	
	mLogo.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_LOGO));
	// origin to centre
	mLogo.setOrigin(((sf::Vector2f)(mpApp->resources.GetTextureAt(Resources::TEXTURE_LOGO).getSize())) * 0.5f);
	
	// position logo and tiled logo
	mLogo.setPosition(gc::VIEWPORT_WIDTH_F * 0.5f, gc::VIEWPORT_HEIGHT_F * logoRelPosY);
	mLogoStretch.setPosition(0.0f, gc::VIEWPORT_HEIGHT_F * logoRelPosY);

	mButtons.at(BTN_SINGLE_GAME).Init("Single Player Game", 0.5f);
	mButtons.at(BTN_HOST_GAME).Init("Host Game", 0.65f);
	mButtons.at(BTN_JOIN_GAME).Init("Join Game", 0.80f);
}

void MainMenuState::Update(float dt)
{
	
}

void MainMenuState::Render(float dt)
{
	mpApp->window.draw(mLogoStretch);
	mpApp->window.draw(mLogo);

	for (auto& b : mButtons)
	{
		b.Render(dt);
	}
}

void MainMenuState::MouseInput(const sf::Vector2f& pos)
{
	if (mButtons.at(BTN_SINGLE_GAME).Contains(pos))
	{
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_PLAYER_SELECTION);
		mpApp->resources.Play(Resources::SOUND_CLICK,mpApp->masterVolume);
	}

	if (mButtons.at(BTN_HOST_GAME).Contains(pos))
	{
		// todo start hosting
		//mpApp->stateManager.ChangeState(???);
		mpApp->gameType = ReversiSFML::GameType::HOST;
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_NETWORK, true);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
	}

	if (mButtons.at(BTN_JOIN_GAME).Contains(pos))
	{
		mpApp->gameType = ReversiSFML::GameType::JOIN;
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_NETWORK, true);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
	}
}

void MainMenuState::KeyInput(sf::Keyboard::Key key)
{
}

void MainMenuState::TextEntered(unsigned int key)
{
}
