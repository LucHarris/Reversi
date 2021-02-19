#include "Board.h"
#include <cmath>


#define PLAYER_CELL_CHAR ((mPlayerIndex == PLAYER_ONE) ? CELL_WHITE : CELL_BLACK)

void Board::SwapPlayers()
{
	std::swap(mPlayerIndex, mOpponentIndex);
}

void Board::GenerateScoreGrid()
{
	std::cout << "\n\n\nGenerateScoreGrid called\n\n\n";

	mScoreGrid.fill(ZERO_SCORE); // reset

	Vector2i cell; // Zero
	int score = 0;

	const char Active = (mPlayerIndex == PLAYER_ONE) ?   CELL_WHITE: CELL_BLACK;
	const char Opponent = (mPlayerIndex == PLAYER_ONE) ? CELL_BLACK : CELL_WHITE;

	// every cell
	while (cell.x < GRID_SIZE)
	{
		while (cell.y < GRID_SIZE)
		{
			if (mDiscGrid.at(cell) == CELL_EMPTY)
			{
				Vector2i offset = cell;

				//std::cout << "\n\ncell:" << cell.x << "," << cell.y << " ";//debug

				for (auto& direction : Vector2i::Directions)
				{
					// first step in direction (reset)
					offset = cell + direction;
					// reset score
					score = 1;

					//std::cout	<< "\n\t direction:" << direction.x << "," << direction.y << "\t"; //debug

					// within limits and next cell in direction is oppenents disc
					while (WithinLimits(offset) && mDiscGrid.at(offset) == Opponent)
					{
						offset += direction;
						//std::cout << "\t offset: " << offset.x << "," << offset.y; //debug
						++score;
					}

					// add score if valid move otherwise ignore
					if (WithinLimits(offset) && score >= MIN_SCORE)
					{
						if (mDiscGrid.at(offset) == Active)
						{
							mScoreGrid.at(cell) += score;
							//std::cout << "\t score added: " << score;//debug
						}
					}

				}
				//std::cout << "\n\t\tcumulative score @ cell:" << mScoreGrid.at(cell)<< "\n\n";//debug
			}
			
			

			++cell.y;
		}
		cell.y = 0;
		++cell.x;
	}
}

void Board::Move(const Vector2i& v)
{
	if (ValidCellForMove(v))
	{
		PlaceMove(v);

		SwapPlayers();

		GenerateScoreGrid();

	}
}

bool Board::ValidCellForMove(const Vector2i& v)
{
	return mScoreGrid.at(v) >= MIN_SCORE;
}

bool Board::CanMove()
{
	bool canMove = false;

	Vector2i v{ 0,0 };
	while (v.x < GRID_SIZE && !canMove)
	{
		while (v.y < GRID_SIZE && !canMove)
		{
			if (mScoreGrid.at(v) > 0)
			{
				canMove = true;
			}
			++v.y;
		}
		++v.x;
	}

	return canMove;
}

void Board::PlaceMove(const Vector2i& v)
{
	if (mScoreGrid.at(v) >= MIN_SCORE)
	{
		Vector2i offset = v;
		int distance = 0;
		const char Active = (mPlayerIndex == PLAYER_ONE) ?	CELL_WHITE: CELL_BLACK;
		const char Opponent = (mPlayerIndex == PLAYER_ONE) ? CELL_BLACK : CELL_WHITE;

		for (auto& direction : Vector2i::Directions)
		{
			// first step in direction (reset)
			offset = v;
			offset += direction;
			// reset count
			distance = 1;

			// within limits and next cell in direction is oppenents disc
			while (WithinLimits(offset) && mDiscGrid.at(offset) == Opponent)
			{
				offset += direction;
				++distance;
			}

			// valid move
			if (WithinLimits(offset))
			{
				if (mDiscGrid.at(offset) == Active)
				{
					// travel back to v changing discs including the choosen position
					for (; distance > 0; --distance)
					{
						offset -= direction;
						mDiscGrid.at(offset) = Active;
					}
				}
			}
		}
	}
	
}

void Board::UpdateBoardBackup()
{
	// override old buffer
	mDiscGridBackup = mDiscGrid;
}

void Board::ToConsole()
{
	std::cout
		<< "\n--------------------\n"
		<< "Player " << PLAYER_CELL_CHAR << " Turn... mPlayerIndex = " << mPlayerIndex << " "
		<< "\n--------------------\n"
		;


	mDiscGrid.to_console();
	std::cout << "\n";
	mDiscGridBackup.to_console();

	mScoreGrid.to_console();
	std::cout << "\n";
}

void Board::PopulateStart()
{
	mDiscGrid = DiscGrid();

	mDiscGrid.at(1, 2) = CELL_WHITE;
	mDiscGrid.at(2, 1) = CELL_WHITE;
	mDiscGrid.at(2, 2) = CELL_BLACK;
	mDiscGrid.at(1, 1) = CELL_BLACK;

}

