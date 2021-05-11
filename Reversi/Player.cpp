#include "Player.h"
#include "Node.h"

Player::Player()
	:
	type(Type::DEFAULT)
{
}

Player::Player(const Player::Type& t)
	:
	type(t)
{
}

int Player::EvaluateMove(const ScoreGrid& sg)
{
	//auto r = std::minmax_element(sg.begin(), sg.end());
	auto r = std::max_element(sg.begin(), sg.end());

	int a = r - sg.begin();

	return a;
}

int Player::EvaluateMoveFromNode(const ReversiBoard& board)
{
	int black = 1;
	int move = -1;
	Node nodeBase(black, move, 2, board);

	return -1;
}
