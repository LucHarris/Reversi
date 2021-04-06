#pragma once
#include <SFML/System/Vector2.hpp>
#include "Board.h"

class Player
{
public:		
	enum class Type{HUMAN,AI};
	Type type;
	// determines strategy
	// simple max position
	int EvaluateMove(const ScoreGrid& sg);
	int EvaluateMoveFromNode(const ReversiBoard& board);

};

