#include "GameSampleState.h"
#include "ReversiSFML.h"
#include "Constants.h"
GameSampleState::GameSampleState(ReversiSFML* app)
	:
	State(app),
	mDiscSprites(app)
{

}

void GameSampleState::Init()
{
	// apply texture and position at centre of viewport
	mBoardSprite.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_BOARD));
	mBoardSprite.setOrigin(mpApp->resources.GetTextureCenter(Resources::TEXTURE_BOARD));
	mBoardSprite.setPosition(gc::VIEWPORT_CENTER);

	mDiscSprites.Init();

}

void GameSampleState::Update(float dt)
{
	mDiscSprites.Update(dt);
}

void GameSampleState::Render(float dt)
{
	mpApp->window.draw(mBoardSprite);
	mDiscSprites.Render(dt);
}

void GameSampleState::MouseInput(const sf::Vector2i& pos)
{
	mpApp->stateManager.ChangeState(gc::STATE_INDEX_MAIN_MENU);
}

void GameSampleState::KeyInput(sf::Keyboard::Key key)
{
}
