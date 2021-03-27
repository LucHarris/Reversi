#include "MainMenuState.h"
#include <assert.h>
#include "ReversiSFML.h"
#include "Constants.h"

MainMenuState::MainMenuState(ReversiSFML* app)
	:
	State(app),
	mButtons{app,app}
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

	mButtons.at(0).Init("Start 1 Player Game", 0.5f);
	mButtons.at(1).Init("Start 2 Player Game", 0.65f);
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
	if (mButtons.at(0).Contains(pos))
	{
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_GAME_SAMPLE);
		mpApp->resources.Play(Resources::SOUND_CLICK,mpApp->masterVolume);
	}
	else
	{

	}
}

void MainMenuState::KeyInput(sf::Keyboard::Key key)
{
}

void MainMenuState::TextEntered(unsigned int key)
{
}
