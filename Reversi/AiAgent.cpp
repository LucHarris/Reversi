#include "AiAgent.h"
#include "Constants.h"

void AiAgent::Init()
{
}

int AiAgent::EvaluateGame(ReversiBoard board)
{
    // get all moves
    // play all moves
    // for all moves, play opponents moves
    // after the two moves, get scores

    std::vector<std::vector<int>> strategies;

    int depth = 0;

    int cell = 0;
    SimulateMove(depth, cell, board,strategies);

    return 0;
}

void AiAgent::SimulateMove(int depth, int cell, ReversiBoard board, std::vector<std::vector<int>>& strategies)
{
    ScoreGrid scoreGrid = board.GetScoreGrid();
    // op turn
    for (size_t c = 0; c < scoreGrid.size(); ++c)
    {
        if (scoreGrid.at(c) > MIN_SCORE)
        {
            board.Move(c);
            SimulateMove(depth + 1,cell, board,strategies);
        }
    }
}



