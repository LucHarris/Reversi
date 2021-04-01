#include "GameSampleState.h"
#include "ReversiSFML.h"
#include "Constants.h"

#include "NormalForm.h"

void GameSampleState::IncActivePlayer()
{
	if (++mActivePlayer >= (int)mPlayers.size())
	{
		mActivePlayer = 0;
	}

	if (mPlayers.at(mActivePlayer).type == Player::Type::AI)
	{
		mAiTimer.Restart(0.3f);
	}
}

void GameSampleState::GameEnded()
{
	mActivePlayer = -1;

	std::string str("White:\t");

	const auto scores(mpApp->reversiGame.GetPlayerScores());
	str += std::to_string(scores.first);
	str += "\nBlack:\t";
	str += std::to_string(scores.second);
	mEndText.setString(str);

	const auto r = mEndText.getGlobalBounds();
	mEndText.setOrigin(r.width * gc::HALF, r.height * gc::HALF);
	mEndText.setPosition(gc::VIEWPORT_CENTER);

}

bool GameSampleState::IsActiveGame()
{
	return mActivePlayer >= 0;
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
	mPlayers.at(0).type = Player::Type::AI; // white
	mPlayers.at(1).type = Player::Type::HUMAN; // black
	mActivePlayer = 0;  // white goes first

	mEndText.setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mEndText.setString("");
}

void GameSampleState::Update(float dt)
{
	if (IsActiveGame())
	{
		mAiTimer.Update(dt);

		if (mPlayers.at(mActivePlayer).type == Player::Type::AI)
		{
			if (mAiTimer.HasElapsed())
			{
				// todo remove nodes
				// const int move = mPlayers.at(mActivePlayer).EvaluateMove(mpApp->reversiGame.GetScoreGrid());
				// const int moveTest = mPlayers.at(mActivePlayer).EvaluateMoveFromNode(mpApp->reversiGame);;

				NormalForm nf(mActivePlayer, mpApp->reversiGame);

				const int dominantMove = nf.Dominant();

				// successful move 
				if (mDiscSprites.Move(dominantMove))
				{
					if (mpApp->reversiGame.CanMove())
					{
						IncActivePlayer();
					}
					else
					{
						GameEnded();
					}
				}
				else
				{
					assert(false && "AI should always have make a valid more if there is one");
				}
			}
		}
	}
	else
	{

	}
	

	mDiscSprites.Update(dt);
}

void GameSampleState::Render(float dt)
{
	mpApp->window.draw(mBoardSprite);
	mDiscSprites.Render(dt);
	mpApp->window.draw(mEndText);

}

void GameSampleState::MouseInput(const sf::Vector2f& pos)
{
	if (IsActiveGame())
	{
		if (mPlayers.at(mActivePlayer).type == Player::Type::HUMAN && mpApp->reversiGame.CanMove())
		{
			const int move = mDiscSprites.MoveByMoooouse(pos);;

			// successful move
			if (move >= 0)
			{
				// try again if move invalid
				if (mDiscSprites.Move(move))
				{
					// next player or game ended
					if (mpApp->reversiGame.CanMove())
					{
						IncActivePlayer();
					}
					else
					{
						GameEnded();
					}
				}
			}
		}

		//mDiscSprites.MoveByMousePos(pos);
		//mpApp->stateManager.ChangeState(gc::STATE_INDEX_MAIN_MENU);
	}

	
}

void GameSampleState::KeyInput(sf::Keyboard::Key key)
{
}
