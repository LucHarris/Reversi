#pragma once

#include <vector>
#include <array>
#include "Player.h"

class PlayerManager
{
	// players are either on white or black
	std::array<std::vector< Player>, 2> mPlayerSides;
	// position in player list
	std::array<int, 2> mPosition;
	// white 0, black 1, starts with 0
	int mActiveSide = 0;
public:
	// side/colour match active game 
	void Increment();

	// ref to active player
	Player& GetActivePlayer();

	void AddPlayer(const Player::Type& t, int side);

	int GetSide() const 
	{
		return mActiveSide;
	}
	// 
	void EndGame()
	{
		mActiveSide = -1;
	}

	bool HasGameEnded() const
	{
		return mActiveSide < 0;
	}

	std::string GetPlayerList(int side);

	// makes sure there is al least a player on each side
	void ValidatePlayers();

	// empty player list
	void ResetSide(int side);
	// removes last from list
	// @return successful removal
	bool RemoveLast(int side);
};
