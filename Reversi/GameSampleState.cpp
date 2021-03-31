#include "GameSampleState.h"
#include "ReversiSFML.h"
#include "Constants.h"

#include "NormalForm.h"

void GameSampleState::IncActivePlayer()
{
	if (++mActivePlayer >= mPlayers.size())
	{
		mActivePlayer = 0;
	}
}

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

	// simple human vs ai setup 
	mPlayers.resize(2);
	mPlayers.at(0).type = Player::Type::HUMAN; // white
	mPlayers.at(1).type = Player::Type::AI; // black
	mActivePlayer = 0;  // white goes first

}

void GameSampleState::Update(float dt)
{
	mAiTimer.Update(dt);

	if (mPlayers.at(mActivePlayer).type == Player::Type::AI)
	{
		if (mAiTimer.HasElapsed())
		{

			const int move = mPlayers.at(mActivePlayer).EvaluateMove(mpApp->reversiGame.GetScoreGrid());

			const int moveTest = mPlayers.at(mActivePlayer).EvaluateMoveFromNode(mpApp->reversiGame);;

			NormalForm nf(mActivePlayer, mpApp->reversiGame);

			int mmm = nf.Dominant();

			// successful move 
			if (mDiscSprites.Move(mmm))
			{
				IncActivePlayer();
			}
		}

	}

	mDiscSprites.Update(dt);
}

void GameSampleState::Render(float dt)
{
	mpApp->window.draw(mBoardSprite);
	mDiscSprites.Render(dt);
}

void GameSampleState::MouseInput(const sf::Vector2f& pos)
{

	if (mPlayers.at(mActivePlayer).type == Player::Type::HUMAN && mpApp->reversiGame.CanMove())
	{
		const int move = mDiscSprites.MoveByMoooouse(pos);;

		// successful move
		if (move >= 0)
		{
			if (mDiscSprites.Move(move))
			{
				IncActivePlayer();

				if (mPlayers.at(mActivePlayer).type == Player::Type::AI)
				{
					mAiTimer.Restart(1.0f);
				}
			}
		}

		
	}
	//mDiscSprites.MoveByMousePos(pos);


	//mpApp->stateManager.ChangeState(gc::STATE_INDEX_MAIN_MENU);
}

void GameSampleState::KeyInput(sf::Keyboard::Key key)
{
}
