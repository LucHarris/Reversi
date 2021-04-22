#include "GameSampleState.h"
#include "ReversiSFML.h"
#include "Constants.h"

#include "NormalForm.h"
#include "Utility.h"
#include "Timer.h"

void GameSampleState::IncActivePlayer()
{
	mPlayers.Increment();

	if (mPlayers.GetActivePlayer().type == Player::Type::AI)
	{
		mAiTimer.Restart(0.00f);
	}
}

void GameSampleState::GameEnded()
{
	mPlayers.EndGame();

	std::string str("White:\t");

	const auto scores(mpApp->reversiGame.GetPlayerScores());
	str += std::to_string(scores.first);
	str += "\nBlack:\t";
	str += std::to_string(scores.second);
	mEndText.setString(str);

	const auto r = mEndText.getGlobalBounds();
	mEndText.setOrigin(r.width * gc::HALF, r.height * gc::HALF);
	mEndText.setPosition(gc::VIEWPORT_CENTER);



	// file out

	if (scores.first > scores.second)
	{
		++mpApp->localUser.whiteWin;
	}
	else
	{
		if (scores.first < scores.second)
		{
			++mpApp->localUser.blackWin;
		}
		else
		{
			++mpApp->localUser.draw;
		}
	}

	++mpApp->localUser.gamesPlayed;

	util::saveFile(gc::PATH_LOCAL_USER, mpApp->localUser);


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

	mEndText.setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mEndText.setString("");

	mPayoffMulti.Load();
}

void GameSampleState::Update(float dt)
{
	if (!mPlayers.HasGameEnded())
	{
		mAiTimer.Update(dt);

		if (mPlayers.GetActivePlayer().type == Player::Type::AI)
		{
			// small delay so not instant
			if (mAiTimer.HasElapsed())
			{
				// todo remove nodes
				// const int move = mPlayers.at(mActivePlayer).EvaluateMove(mpApp->reversiGame.GetScoreGrid());
				// const int moveTest = mPlayers.at(mActivePlayer).EvaluateMoveFromNode(mpApp->reversiGame);;

				size_t pp = PayoffMultipliers::EVEN;

				if (mPlayers.GetSide())
				{
					pp = PayoffMultipliers::ADAPTIVE;
				}

				NormalForm nf(mPayoffMulti.GetAt(pp));

				auto moves = mpApp->reversiGame.GetMoveHistory();


				nf.Evalualate(mPlayers.GetSide(), mpApp->reversiGame, mpApp->reversiGame.GetLastMove());
				Timer<int,std::micro> analysisTimer;

				const int dominantMove = nf.Dominant();

				const int endTime = analysisTimer.elapsed();

				std::ofstream timerOutFile("Data/Out/aiTimer.csv", std::ios::app);
				if (timerOutFile.is_open())
				{
					timerOutFile << endTime << ',' << mpApp->reversiGame.AvailableMoveCount() << '\n';
					timerOutFile.close();
				}
				else
				{
					assert(false);
				}


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
		// todo only called once but remove from loop if possible
		mpApp->reversiGame.ExportWinningMoves();
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
	if (!mPlayers.HasGameEnded())
	{
		if (mPlayers.GetActivePlayer().type == Player::Type::HUMAN && mpApp->reversiGame.CanMove())
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

void GameSampleState::Reset()
{
	mEndText.setString("");
	mpApp->reversiGame.Initialize();
	mPlayers = mpApp->PlayerSelection;
}
