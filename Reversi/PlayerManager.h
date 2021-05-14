#pragma once

#include <vector>
#include <array>
#include "Player.h"

class ReversiSFML;

class PlayerManager
{
	// players are either on white or black
	std::array<std::array<int, 8>, 2> mPlayerSides ;
	// local, host, client, ai
	std::array<Player, 8> mPlayers;
	// iterator for player sides
	std::array<int, 2> mSideCount;
	// position in player list
	std::array<int, 2> mPosition;
	// white 0, black 1, starts with 0
	int mActiveSide = 0;
public:
	PlayerManager();

	// side/colour match active game 
	void Increment();

	// ref to active player
	Player& GetActivePlayer();

	//bool AddPlayer(const Player::Type& t, int side);
	bool AddPlayer(const Player&);
	bool PlayerToSide(int playerIndex, int side);
	bool PlayerToSide(const Player& pl, int side);
	int GetAiPlayerIndex() const;
	// Searches for players by id. return found returns success
	bool PlayerPresent(const Player& pl) const;
	// Searchs by player id. invalid result returns a negative value
	int GetPlayerIndex(const Player& pl) const;

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

	std::string GetPlayerListString(int side);

	// makes sure there is al least a player on each side before starting a game
	void ValidatePlayers();

	// empty player list
	void ResetSide(int side);
	// removes last from list
	// @return successful removal
	bool RemoveLast(int side);

	const std::array<Player, 8>& GetPlayerList() const
	{
		return mPlayers;
	}

	std::vector<Player> GetHumanPlayers() const;

	// draw is -1
	void IncrementWinnerData(int side);

	// updates param with element matching id in player list.
	// for local user data
	bool PlayerListToLocalUser(Player& localUser) const;

	// param updates element in player list. match with id

	bool PlayerUpdatesPlayerList(Player& localUser);
	// 
};

