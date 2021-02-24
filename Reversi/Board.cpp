#include "Board.h"
#include <cmath>
#include <limits>

void ReversiBoard::SwapPlayers()
{
	std::swap(mPlayerIndex, mOpponentIndex);
}

void ReversiBoard::GenerateScoreGrid()
{
	std::cout << "\n\n\nGenerateScoreGrid called\n\n\n";

	Vector2i cell; // Zero

	// reset max and min to out of bounds. 
	mMinMax.first.position = { -1,-1 };
	mMinMax.first.score = MAX_SCORE;	// impssible to get flip the whole board in one move
	mMinMax.first.player = mPlayerIndex;

	mMinMax.second.position = { -1,-1 }; 
	mMinMax.second.score = ZERO_SCORE;
	mMinMax.second.player = mPlayerIndex;

	mScoreGrid.fill(ZERO_SCORE); // reset

	int score = 0;

	const char Active = GetActivePlayerDisc();
	const char Opponent = GetActiveOpponentDisc();

	// every cell
	while (cell.y < GRID_SIZE)
	{
		while (cell.x < GRID_SIZE)
		{
			if (mDiscGrid.at(cell) == CELL_EMPTY)
			{
				Vector2i offset = cell;

				//std::cout << "\n\ncell:" << cell.x << "," << cell.y << " ";//debug

				// look in every direction
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

				// update min max
				const int cellScore = mScoreGrid.at(cell); // can't use local score variable as may be different to cell score

				if (cellScore >= MIN_SCORE)
				{
					// min
					if (cellScore < mMinMax.first.score)
					{
						mMinMax.first.position = cell;
						mMinMax.first.score = cellScore;
					}

					// max
					if (cellScore > mMinMax.second.score)
					{
						mMinMax.second.position = cell;
						mMinMax.second.score = cellScore;
					}

				}
			}
	
			++cell.x;
		}
		cell.x = 0;
		++cell.y;
	}
}

void ReversiBoard::Move(const Vector2i& v)
{
	if (ValidCellForMove(v))
	{
		PlaceMove(v);

		SwapPlayers();

		GenerateScoreGrid();

	}
}

bool ReversiBoard::ValidCellForMove(const Vector2i& v)
{
	return mScoreGrid.at(v) >= MIN_SCORE;
}

bool ReversiBoard::CanMove()
{
	return mMinMax.first.IsValid() && mMinMax.second.IsValid();
}

void ReversiBoard::PlaceMove(const Vector2i& v)
{
	if (mScoreGrid.at(v) >= MIN_SCORE)
	{
		Vector2i offset = v;
		int distance = 0;
		const char Active = GetActivePlayerDisc();
		const char Opponent = GetActiveOpponentDisc();

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

void ReversiBoard::UpdateBoardBackup()
{
	// override old buffer
	mDiscGridBackup = mDiscGrid;
}

void ReversiBoard::ToConsole()
{
	std::cout
		<< "\n--------------------\n"
		<< "Player " << GetActivePlayerDisc() << " Turn... mPlayerIndex = " << mPlayerIndex << " "
		<< "\n--------------------\n"
		;

	mDiscGrid.to_console();
	std::cout << "\n";
	mDiscGridBackup.to_console();
	std::cout << "\n";

	mScoreGrid.to_console();
	std::cout << "\nMinMax:";
	std::cout
		<< "\n\tMin Score: " << mMinMax.first.score << "\tPos:" << mMinMax.first.position.x << "," << mMinMax.first.position.y << "\n"
		<< "\n\tMax Score: " << mMinMax.second.score << "\tPos:" << mMinMax.second.position.x << "," << mMinMax.second.position.y << "\n"
		;
	std::cout << "\n";
}

const char ReversiBoard::GetActivePlayerDisc() const 
{
	return (mPlayerIndex == PLAYER_ONE) ? CELL_WHITE : CELL_BLACK;
}

const char ReversiBoard::GetActiveOpponentDisc() const
{
	return (mPlayerIndex == PLAYER_ONE) ? CELL_BLACK : CELL_WHITE;
}

void ReversiBoard::Initialize()
{
	PopulateStart();
	GenerateScoreGrid();
}

void ReversiBoard::PopulateStart()
{
	mDiscGrid = DiscGrid();

	mDiscGrid.at(1, 2) = CELL_WHITE;
	mDiscGrid.at(2, 1) = CELL_WHITE;
	mDiscGrid.at(2, 2) = CELL_BLACK;
	mDiscGrid.at(1, 1) = CELL_BLACK;

}

const bool MoveData::IsValid() const
{
	return
		position.x >= 0 &&
		position.y >= 0 &&
		position.x < GRID_SIZE&&
		position.y < GRID_SIZE&&
		score >= MIN_SCORE && score <= MAX_SCORE;
}

const ReversiBoard::MinMax& ReversiBoard::GetMinMax() const
{
	return mMinMax;
}

const std::pair<int, int> ReversiBoard::GetPlayerScores() const
{
	return { mDiscGrid.count(CELL_WHITE) ,  mDiscGrid.count(CELL_BLACK) };
}
