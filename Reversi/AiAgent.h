#pragma once
#include "Board.h"
#include <vector>



struct StategyScore
{
	int location[2],		score[2];
};

typedef std::vector<StategyScore> StrategyScores;

class AiAgent
{
	ScoreGrid mCornerMultiplier;
public:

	void Init();
	// @param take a copy of the board to simulate the topponents following move
	int EvaluateGame(ReversiBoard board);

	void SimulateMove(int depth, int cell, ReversiBoard board , std::vector<std::vector<int>>& strategies);

};

