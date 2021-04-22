#include "PlayerManager.h"
#include <cassert>
#include "Constants.h"

void PlayerManager::Increment()
{
	// side through sides
	if (++mActiveSide >= (int)mPlayerSides.size())
	{
		mActiveSide = 0;
	}

	// cycle through players on active side
	if (++mPosition.at(mActiveSide) >= (int)mPlayerSides.at(mActiveSide).size())
	{
		mPosition.at(mActiveSide) = 0;
	}

}

Player& PlayerManager::GetActivePlayer()
{
	// within boundary
	assert(mActiveSide < (int)mPlayerSides.size());
	assert(mPosition.at(mActiveSide) < (int)mPlayerSides.at(mActiveSide).size());

	return mPlayerSides.at(mActiveSide).at( mPosition.at(mActiveSide) );
}

bool PlayerManager::AddPlayer(const Player::Type& t, int side)
{
	assert(side < (int)mPlayerSides.size());
	if (mPlayerSides.at(side).size() < gc::MAX_PLAYERS_PER_SIDE)
	{
		mPlayerSides.at(side).push_back(t);
		return true;
	}
	else
	{
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
		playerList += (p.type == Player::Type::AI) ? "CPU" : "Player";
	}

	return playerList;
}

void PlayerManager::ValidatePlayers()
{
	if (mPlayerSides.at(0).size() == 0)
	{
		AddPlayer(Player::Type::AI, 0);
	}

	if (mPlayerSides.at(1).size() == 0)
	{
		AddPlayer(Player::Type::AI, 1);
	}
}

bool PlayerManager::RemoveLast(int side)
{
	assert(side < (int)mPlayerSides.size());

	if (mPlayerSides.at(side).size() > 0)
	{
		mPlayerSides.at(side).pop_back();
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

	mPlayerSides.at(side).clear();
}


