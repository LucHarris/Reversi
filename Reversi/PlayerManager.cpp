#include "PlayerManager.h"
#include <cassert>

void PlayerManager::Increment()
{
	// side through sides
	if (++mActiveSide >= mPlayerSides.size())
	{
		mActiveSide = 0;
	}

	// cycle through players on active side
	if (++mPosition.at(mActiveSide) >= mPlayerSides.at(mActiveSide).size())
	{
		mPosition.at(mActiveSide) = 0;
	}

}

Player& PlayerManager::GetActivePlayer()
{
	// within boundary
	assert(	mActiveSide < mPlayerSides.size() &&
			mPosition.at(mActiveSide) < mPlayerSides.at(mActiveSide).size());

	return mPlayerSides.at(mActiveSide).at( mPosition.at(mActiveSide) );
}

void PlayerManager::AddPlayer(const Player::Type& t, int side)
{
	assert(side < mPlayerSides.size());

	mPlayerSides.at(side).push_back(t);
}


