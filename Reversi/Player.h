#pragma once
#include <SFML/System/Vector2.hpp>
#include "Board.h"
class Player
{
public:		
	enum class Type{HUMAN,AI};
	Type type;
	// determines strategy
	// todo more parameters for previous moves?
	int EvaluateMove(const ScoreGrid& sg);

};

