#include "ReversiSFML.h"
#include "PlayerManager.h"
#include <cassert>
#include "Constants.h"
#include <sstream>
#include <algorithm>
#include <set>

PlayerManager::PlayerManager()
{
	for (auto& side : mPlayerSides)
	{
		for (auto& pos : side)
		{
			pos = -1;
		}
	}

	mSideCount.at(0) = 0;
	mSideCount.at(1) = 0;
	mPosition.at(0) = 0;
	mPosition.at(1) = 0;
}

void PlayerManager::Increment()
{
	// cycle through players on active side
	if (++mPosition.at(mActiveSide) >=   mSideCount.at(mActiveSide))
	{
		mPosition.at(mActiveSide) = 0;
	}

	// toggles between sides
	mActiveSide = 1 - mActiveSide;
}

Player& PlayerManager::GetActivePlayer()
{
	// within boundary
	assert(mActiveSide < (int)mPlayerSides.size()); // less than 2
	assert(mPosition.at(mActiveSide) < mSideCount.at(mActiveSide));// within active player range

	return  mPlayers.at( mPlayerSides.at(mActiveSide).at( mPosition.at(mActiveSide) ) );
}

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

	// cannot find player in mpSelectionPlayers
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

bool PlayerManager::PlayerPresent(const Player& pl) const
{
	// match by id as stats may be outdated
	auto it = find_if(mPlayers.begin(), mPlayers.end(), [&pl](const Player& p)
		{
			return p.userData.id == pl.userData.id;
		});
	
	return it != mPlayers.end();
}


int PlayerManager::GetPlayerIndex(const Player& pl) const
{
	// finds player in list by id
	auto it = find_if(mPlayers.begin(), mPlayers.end(), [&pl](const Player& p)
		{
			return p.userData.id == pl.userData.id;
		});

	// gets index in player list.
	const int index = (it != mPlayers.end())? std::distance(mPlayers.begin(), it) : -1;

	return index;
}

std::string PlayerManager::GetPlayerListString(int side)
{
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
		}

		
		
	}

	return playerList.str();
}

void PlayerManager::ValidatePlayers()
{
	for (int i = 0; i < (int)mSideCount.size(); ++i)
	{
		assert(i < (int)mPlayerSides.size());
		if (mSideCount.at(i) == 0)
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

void PlayerManager::IncrementWinnerData(int side)
{
	std::set<int> modifyPlayers;

	// updates winner data
	switch (side)
	{
	case PLAYER_ONE:
		// avoid duplication
		for (auto& wPlayer : mPlayerSides.at(side))
		{
			if (wPlayer >= 0)
			{
				if (mPlayers.at(wPlayer).type == Player::Type::HUMAN)
				{
					modifyPlayers.insert(wPlayer);
				}
			}
		}
		for (auto& wPlayer : modifyPlayers)
		{
			++mPlayers.at(wPlayer).userData.whiteWin;
		}
		break;
	case PLAYER_TWO:
		// avoid duplication
		for (auto& bPlayer : mPlayerSides.at(side))
		{
			// valid player
			if (bPlayer >= 0)
			{
				if (mPlayers.at(bPlayer).type == Player::Type::HUMAN)
				{
					modifyPlayers.insert(bPlayer);
				}

			}
		}

		for (auto& wPlayer : modifyPlayers)
		{
			++mPlayers.at(wPlayer).userData.blackWin;
		}

		break;
	case -1: // draw
		for (auto& side : mPlayerSides)
		{
			for (auto& dPlayer : side)
			{
				if (dPlayer >= 0)
				{
					if (mPlayers.at(dPlayer).type == Player::Type::HUMAN)
					{
						modifyPlayers.insert(dPlayer);
					}
				}
			}
		}

		for (auto& dPlayer : mPlayerSides.at(side))
		{
			++mPlayers.at(dPlayer).userData.draw;
			++mPlayers.at(dPlayer).userData.gamesPlayed;

		}

		break;
	default:
		assert(false);
		break;
	}

	// increment games played and total wins
	switch (side)
	{
	case PLAYER_ONE:;
	case PLAYER_TWO:

		modifyPlayers.clear();
		for (auto& player : mPlayerSides.at(side))
		{
			if (player >= 0)
			{
				if (mPlayers.at(player).type == Player::Type::HUMAN)
				{
					++mPlayers.at(player).userData.gamesPlayed;
					mPlayers.at(player).userData.totalWins = mPlayers.at(player).userData.whiteWin + mPlayers.at(player).userData.blackWin;
				}
			}
		}
		break;
	case -1:
		// already incremented games played and total wins don't change
		break;
	default:
		assert(false);
		break;
	}
}

std::string PlayerManager::DebugSideInfo()
{
	std::ostringstream oss;
	if (!HasGameEnded())
	{
		oss
			<< "\nWhite pos: " << mPosition.at(0)
			<< "\tBlack pos:" << mPosition.at(1)
			<< "\nWhite sCount: " << mSideCount.at(0)
			<< "\tBlack sCount:" << mSideCount.at(1)
			<< "\nActive Side:" << mActiveSide
			<< "\tActive Pos@side:" << mPosition.at(mActiveSide)
			<< "\nActive id:" << mPlayers.at(mPlayerSides.at(mActiveSide).at(mPosition.at(mActiveSide))).userData.id
			<< "\nActive nm:" << mPlayers.at(mPlayerSides.at(mActiveSide).at(mPosition.at(mActiveSide))).userData.name
			<< "\n\n"
			;

		oss
			<< GetPlayerListString(0)
			<< GetPlayerListString(1)
			;
	}
	else
	{
		oss
			<< "game ended";
	}

	return oss.str();
	
}

bool PlayerManager::PlayerListToLocalUser(Player& localUser) const
{
	auto foundUser = std::find_if(mPlayers.begin(), mPlayers.end(), [&localUser](const Player& pl) 
		{
			return localUser.userData.id == pl.userData.id;
		});

	if (foundUser != mPlayers.end())
	{
		localUser = *foundUser;
		return true;
	}
	else
	{
		return false;
	}
}


bool PlayerManager::PlayerUpdatesPlayerList(Player& localUser)
{
	auto foundUser = std::find_if(mPlayers.begin(), mPlayers.end(), [&localUser](const Player& pl)
		{
			return localUser.userData.id == pl.userData.id;
		});

	if (foundUser != mPlayers.end())
	{
		*foundUser = localUser;
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<Player> PlayerManager::GetHumanPlayers() const
{
	std::vector<Player> pl;

	std::copy_if(mPlayers.begin(), mPlayers.end(), std::back_inserter(pl), [](const Player& p)
		{
			return p.IsType(Player::Type::HUMAN);
		});
	
	std::sort(pl.begin(), pl.end(), [](const Player& a, const Player& b) 
		{
			return a.userData.totalWins > b.userData.totalWins;
		});

	return pl;
}

void PlayerManager::ReplacePlayersInSides(int search, int replace)
{
	for (auto& side : mPlayerSides)
	{
		for (auto& pl : side)
		{
			if (search == pl)
			{
				pl = replace;
			}
		}
	}
}
