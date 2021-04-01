#include "NormalForm.h"
#include "Constants.h"
#include <numeric>
void NormalForm::GeneratePayoffs(const ReversiBoard& board, int& p1, int& p2)
{
	const auto& b = board.GetDiscGrid();

	Grid<int, gc::BOARD_DISC_SIZE> payoffBoard;

	for (int i = 0; i < gc::BOARD_DISC_SIZE; i++)
	{
		// defaults if tile isn't occuied by a disc
		int disc = 0;
		switch (b.at(0))
		{
		case CELL_WHITE: disc = 1;
			break;
		case CELL_BLACK: disc = -1;
			break;
		}

		payoffBoard.at(0) = disc;
	}

	// applies multiplier
	std::transform(
		gc::CORNER_SCORE_MULTPLIER,
		gc::CORNER_SCORE_MULTPLIER + gc::BOARD_DISC_SIZE,
		payoffBoard.begin(),
		payoffBoard.begin(),
		std::multiplies<int>()
	);

	// white payoff sums the positive values for a positive payoff
	p1 = std::accumulate(payoffBoard.begin(), payoffBoard.end(),0, [](int a, int b) 
		{
			if (b > 0)
			{
				return a + b;
			}
			else
			{
				return a;
			}

		});

	// black payoff converts minus values to a positive payoff
	p2 = std::accumulate(payoffBoard.begin(), payoffBoard.end(), 0, [](int a, int b)
		{
			if (b < 0)
			{
				return a - b;
			}
			else
			{
				return a;
			}

		});


}
