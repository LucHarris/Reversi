#include "Node.h"

Node::Node(int agent, int& m, int depth, ReversiBoard board)
	:
	move(m)
{
	--depth;

	ScoreGrid scoreGrid = board.GetScoreGrid();

	// base node doesnt call move
	if (m > 0)
	{
		board.Move(m);

		if (board.BoardHasChanged())
		{
			board.UpdateBoardBackup();
		}
	}

	scores = board.GetPlayerScores();

	// no more node children if 
	if (board.CanMove())
	{
		boardState = BoardState::CAN_MOVE;
		// base condtion
		if (depth >= 0)
		{
			// search for moves
			for (int i = 0; i < (int)scoreGrid.size(); i++)
			{
				if (scoreGrid.at(i) >= MIN_SCORE)
				{
					children.push_back(Node(agent,i, depth, board));
				}
			}
		}
		else
		{
		}
	}
	else
	{
		// assume lost until proven otherwise
		boardState = BoardState::LOSE;

		// determine win or draw state
		if (scores.first == scores.second)
		{
			boardState = BoardState::DRAW;

		}
		else
		{
			// did the agent win
			if (agent ^ (scores.first > scores.second))
			{
				boardState = BoardState::WIN;
			}

		}
	}


}
