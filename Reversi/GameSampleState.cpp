#include "GameSampleState.h"
#include "ReversiSFML.h"
#include "Constants.h"
#include "NormalForm.h"
#include "Utility.h"
#include "Timer.h" 

void GameSampleState::IncActivePlayer()
{
	mpSelectionPlayers->Increment();

	if (mpSelectionPlayers->GetActivePlayer().type == Player::Type::AI)
	{
		mAiTimer.Restart(0.00f);
	}
}

void GameSampleState::GameEnded()
{
	mpSelectionPlayers->EndGame();

	std::string str("White:\t");

	const auto scores(mpApp->reversiGame.GetPlayerScores());
	str += std::to_string(scores.first);
	str += "\nBlack:\t";
	str += std::to_string(scores.second);
	mText.at(TEXT_END).setString(str);

	const auto r = mText.at(TEXT_END).getGlobalBounds();
	mText.at(TEXT_END).setOrigin(r.width * gc::HALF, r.height * gc::HALF);
	mText.at(TEXT_END).setPosition(gc::VIEWPORT_CENTER);

	// no more goes
	mText.at(TEXT_TURN).setString("");

	int winSide = -1; // draw by default

	if (scores.first > scores.second)
	{
		winSide = PLAYER_ONE;
	}
	else
	{
		if (scores.first < scores.second)
		{
			winSide = PLAYER_TWO;
		}
	}

	mpApp->playerSelection.IncrementWinnerData(winSide);
	mpApp->playerSelection.PlayerListToLocalUser(mpApp->localPlayer);

	// output local user data
	util::saveFile(gc::PATH_LOCAL_USER, mpApp->localPlayer.userData);
}

void GameSampleState::UpdateTurnList()
{
	if (!mpApp->playerSelection.HasGameEnded())
	{
		std::ostringstream oss;

		oss << "Next Turn:\n";
		PlayerManager pm = *mpSelectionPlayers;
		for (size_t i = 0; i < 5;)
		{
			oss << ++i
				<< "\t"
				<< pm.GetActivePlayer().userData.name
				<< "\n";
			pm.Increment();
		}

		mText.at(TEXT_TURN).setString(oss.str());
	}
	else
	{
		mText.at(TEXT_TURN).setString("");
	}
	
}

GameSampleState::GameSampleState(ReversiSFML* app)
	:
	State(app),
	mDiscSprites(app),
	mpSelectionPlayers(&app->playerSelection)
{

}

void GameSampleState::Init()
{
	// apply texture and position at centre of viewport
	mBoardSprite.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_BOARD));
	mBoardSprite.setOrigin(mpApp->resources.GetTextureCenter(Resources::TEXTURE_BOARD));
	mBoardSprite.setPosition(gc::VIEWPORT_CENTER);

	mDiscSprites.Init();

	mText.at(TEXT_END).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_END).setString("");

	mText.at(TEXT_TURN).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_END).setString("");

	mPayoffMulti.Load();

	mAiNormalForm[0].Init(mPayoffMulti.GetAt(PayoffMultipliers::PREDEFINED), mPayoffMulti.GetAt(PayoffMultipliers::ADAPTIVE));
	mAiNormalForm[1].Init(mPayoffMulti.GetAt(PayoffMultipliers::PREDEFINED), mPayoffMulti.GetAt(PayoffMultipliers::ADAPTIVE));
}

void GameSampleState::Update(float dt)
{
	if (!mpSelectionPlayers->HasGameEnded())
	{
		mAiTimer.Update(dt);

		// todo move !join logic?
		if (mpApp->gameType != ReversiSFML::GameType::JOIN)
		{
			if (mpSelectionPlayers->GetActivePlayer().IsType( Player::Type::AI))
			{
				// small delay so not instant
				if (mAiTimer.HasElapsed())
				{
					Timer<int, std::micro> analysisTimer;
					const int move = mAiNormalForm[mpSelectionPlayers->GetSide()].Evalualate(mpSelectionPlayers->GetSide(), mpApp->reversiGame, mpApp->reversiGame.GetLastMove());
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
					if (mDiscSprites.Move(move))
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

			UpdateTurnList();
		}
		else
		{
			// client
		}
		
		

	}
	else
	{
		// todo only called once but remove from loop if possible
		mpApp->reversiGame.ExportWinningMoves();
	}
	
	mDiscSprites.Update(dt);

	mDiscSprites.UpdateDiscs();
}

void GameSampleState::Render(float dt)
{
	mpApp->window.draw(mBoardSprite);
	mDiscSprites.Render(dt);
	for (auto& t : mText)
	{
		mpApp->window.draw(t);
	}

}

void GameSampleState::MouseInput(const sf::Vector2f& pos)
{
	if (mpApp->gameType != ReversiSFML::GameType::JOIN)
	{
		if (!mpSelectionPlayers->HasGameEnded())
		{
			// valid active player able to move
			if (mpSelectionPlayers->GetActivePlayer().type == Player::Type::HUMAN &&  
				mpApp->reversiGame.CanMove() && 
				mpSelectionPlayers->GetActivePlayer().userData.id == mpApp->localPlayer.userData.id)
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
		}
	}
	else
	{
		// client input
		const int move = mDiscSprites.MoveByMoooouse(pos);

		// validation
		if (move >= 0 && move < 64)
		{
			ClientSendData sendData;
			sendData.move = move;
			sendData.dummy = false;
			sendData.player = mpApp->localPlayer;

			// send to host
			mpApp->threadPool.PushInputQueue(sendData);
		}
	}

	mpApp->debugLog.setString(mpApp->playerSelection.DebugSideInfo());

	
}

void GameSampleState::KeyInput(sf::Keyboard::Key key)
{
}

void GameSampleState::Reset()
{
	mAiNormalForm[0] = NormalForm();
	mAiNormalForm[1] = NormalForm();

	mAiNormalForm[0].Init(mPayoffMulti.GetAt(PayoffMultipliers::PREDEFINED), mPayoffMulti.GetAt(PayoffMultipliers::ADAPTIVE));
	mAiNormalForm[1].Init(mPayoffMulti.GetAt(PayoffMultipliers::PREDEFINED), mPayoffMulti.GetAt(PayoffMultipliers::ADAPTIVE));

	mText.at(TEXT_END).setString("");

	if (mpApp->gameType != ReversiSFML::GameType::JOIN)
	{
		mpApp->reversiGame.Initialize();
		mpApp->playerSelection.NewGame();
		mDiscSprites.UpdateDiscs();
	}
	else
	{

	}

	mpApp->debugLog.setString(mpApp->playerSelection.DebugSideInfo());

}
