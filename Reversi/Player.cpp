#include "Player.h"

int Player::EvaluateMove(const ScoreGrid& sg)
{
	//auto r = std::minmax_element(sg.begin(), sg.end());
	auto r = std::max_element(sg.begin(), sg.end());

	int a = r - sg.begin();

	return a;
}
