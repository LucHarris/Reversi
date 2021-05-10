#include "PlayerManager.h"
#include <cassert>
#include "Constants.h"

PlayerManager::PlayerManager()
{
}

void PlayerManager::Increment()
{
	// todo test and remove deadcode
	/*if (++mActiveSide >= (int)mPlayerSides.size())
	{
		mActiveSide = 0;
	}*/

	// toggles between sides
	mActiveSide = 1 - mActiveSide;

	// cycle through players on active side
	if (++mPosition.at(mActiveSide) >=   mSideCount.at(mActiveSide))//todo remove   (int)mPlayerSides.at(mActiveSide).size())
	{
		mPosition.at(mActiveSide) = 0;
	}

}

Player& PlayerManager::GetActivePlayer()
{
	// within boundary
	assert(mActiveSide < (int)mPlayerSides.size()); // less than 2
	assert(mPosition.at(mActiveSide) < mSideCount.at(mActiveSide));// within active player range // todo remove dead code(int)mPlayerSides.at(mActiveSide).size());

	return mPlayerSides.at(mActiveSide).at( mPosition.at(mActiveSide) );
}

bool PlayerManager::AddPlayer(const Player::Type& t, int side)
{
	assert(side < (int)mPlayerSides.size() && side >= 0); // 0 or 1

	// adds player
	if (mSideCount.at(side) < gc::MAX_PLAYERS_PER_SIDE)
	{
		mPlayerSides.at(side).at(mSideCount.at(side)) = Player(t);
		++mSideCount.at(side);
		return true;
	}
	else
	{
		// invalid range
		return false;
	}
}

std::string PlayerManager::GetPlayerList(int side)
{
	std::string playerList = "";
	assert(side < (int)mPlayerSides.size());

	switch (side)
	{
	case PLAYER_ONE: 
		playerList += "White"; 
		break;
	case PLAYER_TWO: 
		playerList += "Black"; 
		break;
	default:
		playerList += "???"; 
		break;
	}

	for (auto& p : mPlayerSides.at(side))
	{
		// indent
		playerList += "\n  ";

		// todo add player name

		switch (p.type)
		{
		case Player::Type::AI: 
			playerList += "CPU";
			break;
		case Player::Type::HUMAN: 
			playerList += "Player";
			break;
		case Player::Type::DEFAULT: 
			// nothing
			break;
		default:
			break;
		}
	}

	return playerList;
}

void PlayerManager::ValidatePlayers()
{
	for (int i = 0; i < (int)mSideCount.size(); ++i)
	{
		assert(i < (int)mPlayerSides.size());
		if (mSideCount.at(i) == 0)// (mPlayerSides.at(0).size() == 0)
		{
			AddPlayer(Player::Type::AI, i);
		}
	}
}

bool PlayerManager::RemoveLast(int side)
{
	assert(side < (int)mPlayerSides.size());

	if (mSideCount.at(side) > 0)
	{
		--mSideCount.at(side);
		mPlayerSides.at(side).at(mSideCount.at(side)) = Player(Player::Type::DEFAULT);
		return true;
	}
	else
	{
		return false;
	}
}

void PlayerManager::ResetSide(int side)
{
	assert(side < (int)mPlayerSides.size());

	mSideCount.at(side) = 0;

	for (auto& p : mPlayerSides.at(side))
	{
		p = Player(Player::Type::DEFAULT);
	}
}


