#pragma once
#include <WinSock2.h>
#include <SFML/System/Vector2.hpp>
#include "Board.h"
#include "User.h"

class Player
{
public:		
	enum class Type{HUMAN,AI,DEFAULT};
	Player();
	Player(const Player::Type& t);
	// local data
	UserData userData;
	// assign in server socket before pushing to main thread
	// to compare against existing sockets
	SOCKET serverSocket;
	Type type;
	// determines strategy
	// simple max position
	int EvaluateMove(const ScoreGrid& sg);
	int EvaluateMoveFromNode(const ReversiBoard& board);
	bool IsType(Type t) const;
};

