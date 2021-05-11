#include "PlayerManager.h"
#include <cassert>
#include "Constants.h"
#include <sstream>
#include <algorithm>
PlayerManager::PlayerManager()
{
	for (auto& side : mPlayerSides)
	{
		for (auto& pos : side)
		{
			pos = -1;
		}
	}

}

void PlayerManager::Increment()
{

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

	return  mPlayers.at( mPlayerSides.at(mActiveSide).at( mPosition.at(mActiveSide) ) );
}

//todo remove old member function
//bool PlayerManager::AddPlayer(const Player::Type& t, int side)
//{
//	assert(side < (int)mPlayerSides.size() && side >= 0); // 0 or 1
//
//	// adds player
//	if (mSideCount.at(side) < gc::MAX_PLAYERS_PER_SIDE)
//	{
//		mPlayerSides.at(side).at(mSideCount.at(side)) = Player(t);
//		++mSideCount.at(side);
//		return true;
//	}
//	else
//	{
//		// invalid range
//		return false;
//	}
//}

bool PlayerManager::AddPlayer(const Player& p)
{

	auto it = std::find_if(mPlayers.begin(), mPlayers.end(), [](const Player& pl) 
		{
			return pl.type == Player::Type::DEFAULT;
		});

	if (it == mPlayers.end())
	{
		return false;
	}

	// assign player
	*it = p;

	return true;
}

bool PlayerManager::PlayerToSide(int playerIndex, int side)
{
	assert(playerIndex < (int)mPlayers.size());

	if (mSideCount.at(side) < gc::MAX_PLAYERS_PER_SIDE)
	{
		mPlayerSides.at(side).at(mSideCount.at(side)) = playerIndex;
		++mSideCount.at(side);
		return true;
	}
	else
	{
		// invalid range
		return false;
	}

	return false;
}

bool PlayerManager::PlayerToSide(const Player& pl, int side)
{
	auto it = find_if(mPlayers.begin(), mPlayers.end(), [&pl](const Player& p) 
		{
			return p.userData.id == pl.userData.id;
		});

	// cannot find player in mPlayers
	if (it == mPlayers.end())
	{
		return false;
	}

	const int i = std::distance(mPlayers.begin(), it);

	return PlayerToSide(i, side);
}

int PlayerManager::GetAiPlayerIndex() const
{
	auto it = find_if(mPlayers.begin(), mPlayers.end(), [](const Player& p)
		{
			return p.type == Player::Type::AI;
		});

	// Needs to be an ai in the player list
	assert(it != mPlayers.end());

	return std::distance(mPlayers.begin(), it);
}

std::string PlayerManager::GetPlayerList(int side)
{
	//std::string playerList = "";
	std::ostringstream playerList;
	assert(side < (int)mPlayerSides.size());

	switch (side)
	{
	case PLAYER_ONE: 
		playerList << "White"; 
		break;
	case PLAYER_TWO: 
		playerList << "Black"; 
		break;
	default:
		playerList << "???"; 
		break;
	}

	for (auto& p : mPlayerSides.at(side))
	{
		// indent
		playerList << "\n";

		if (p >= 0)
		{
			playerList << mPlayers.at(p).userData.name;

			if (mPlayers.at(p).type == Player::Type::AI)
			{
				playerList << " (CPU)";
			}
		}

		
		
	}

	return playerList.str();
}

void PlayerManager::ValidatePlayers()
{
	for (int i = 0; i < (int)mSideCount.size(); ++i)
	{
		assert(i < (int)mPlayerSides.size());
		if (mSideCount.at(i) == 0)// (mPlayerSides.at(0).size() == 0)
		{
			int lookup = GetAiPlayerIndex();
			// lookup assigned to side
			PlayerToSide(lookup,i);
		}
	}
}

bool PlayerManager::RemoveLast(int side)
{
	assert(side < (int)mPlayerSides.size());

	if (mSideCount.at(side) > 0)
	{
		--mSideCount.at(side);
		mPlayerSides.at(side).at(mSideCount.at(side)) = -1;
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
		p = -1;
	}
}


