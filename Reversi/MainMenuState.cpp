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
	mLogoStretch.setTextureRect({ 0,0,gc::ViewportWidth,128 });

	
	mLogo.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_LOGO));
	// origin to centre
	mLogo.setOrigin(((sf::Vector2f)(mpApp->resources.GetTextureAt(Resources::TEXTURE_LOGO).getSize())) * 0.5f);
	
	// position logo and tiled logo
	mLogo.setPosition(gc::ViewportWidthF * 0.5f, gc::ViewportHeightF * logoRelPosY);
	mLogoStretch.setPosition(0.0f, gc::ViewportHeightF * logoRelPosY);

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

void MainMenuState::MouseInput(const sf::Vector2i& pos)
{
	if (mButtons.at(0).Contains(pos))
	{
		assert(false);
	}
}

void MainMenuState::KeyInput(sf::Keyboard::Key key)
{
}
