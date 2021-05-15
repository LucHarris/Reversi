#include "Board.h"
#include <cmath>
#include <limits>
#include "Constants.h"
#include <fstream>

void ReversiBoard::SwapPlayers()
{
	std::swap(mPlayerIndex, mOpponentIndex);
}

void ReversiBoard::GenerateScoreGrid()
{
	//std::cout << "\n\n\nGenerateScoreGrid called\n\n\n";

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

bool ReversiBoard::Move(const Vector2i& v)
{
	/*bool m = false;

	if (ValidCellForMove(v))
	{
		m = PlaceMove(v);
		
		mMoveTracker.emplace_back(v);

		SwapPlayers();

		GenerateScoreGrid();

	}

	return m;*/

	return Move((v.y * GRID_SIZE) + v.x);


}

bool ReversiBoard::ValidCellForMove(const Vector2i& v)
{
	bool b = false;

	// within board
	if (v.x >= 0 &&
		v.y >= 0 &&
		v.x < gc::BOARD_DISC_ROW_SIZE &&
		v.y < gc::BOARD_DISC_ROW_SIZE
		)
	{
		// valid move if score above min
		b = mScoreGrid.at(v) >= MIN_SCORE;
	}

	return b;
}

bool ReversiBoard::ValidCellForMove(int a)
{
	bool b = false;

	if (a >= 0 && a < (int)gc::BOARD_DISC_SIZE)
	{
		b = mScoreGrid.at(a) >= MIN_SCORE;
	}

	return b;
}

bool ReversiBoard::CanMove() const 
{
	return mMinMax.first.IsValid() && mMinMax.second.IsValid();
}

bool ReversiBoard::PlaceMove(const Vector2i& v)
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
		return true;
	}
	else
	{
		return false;
	}
	
}

void ReversiBoard::UpdateBoardBackup()
{
	// override old buffer
	//mDiscGridBackup = mDiscGrid;
}

bool ReversiBoard::Move(int a)
{

	// to 2d coordinates
	/*int x = a % 8;
	int y = a / 8;
	return Move({x, y});*/


	bool m = false;

	if (ValidCellForMove(a))
	{
		int x = a % 8;
		int y = a / 8;

		m = PlaceMove({ x, y });

		// update move tracker
		auto f = std::find(mMoveTracker.begin(), mMoveTracker.end(), -1);
		// will always contain at least one -1
		assert(f != mMoveTracker.end());
		// update element with move
		*f = m;
		//todo remove vector deadcode mMoveTracker.emplace_back(a);

		SwapPlayers();

		GenerateScoreGrid();

	}

	return m;

}

void ReversiBoard::ToConsole()
{
	/*std::cout
		<< "\n--------------------\n"
		<< "Player " << GetActivePlayerDisc() << " Turn... mPlayerIndex = " << mPlayerIndex << " "
		<< "\n--------------------\n"
		;

	mDiscGrid.to_console();
	std::cout << "\n";

	mScoreGrid.to_console();
	std::cout << "\nMinMax:";
	std::cout
		<< "\n\tMin Score: " << mMinMax.first.score << "\tPos:" << mMinMax.first.position.x << "," << mMinMax.first.position.y << "\n"
		<< "\n\tMax Score: " << mMinMax.second.score << "\tPos:" << mMinMax.second.position.x << "," << mMinMax.second.position.y << "\n"
		;
	std::cout << "\n";*/
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
	mExportedWinningMoves = false;
	//mMoveTracker.reserve(GRID_SIZE * GRID_SIZE); // won't need more moves than board size
	//mMoveTracker.clear();
	mMoveTracker.fill(-1);
	mDiscGrid.fill(CELL_EMPTY);
	mDiscGridBackup.fill(CELL_EMPTY);
	mScoreGrid.fill(ZERO_SCORE);
	PopulateStart();
	GenerateScoreGrid();
}

void ReversiBoard::PopulateStart()
{
	mDiscGrid;

	mDiscGrid.at(3, 4) = CELL_WHITE;
	mDiscGrid.at(4, 3) = CELL_WHITE;
	mDiscGrid.at(4, 4) = CELL_BLACK;
	mDiscGrid.at(3, 3) = CELL_BLACK;
	mPlayerIndex = PLAYER_ONE; // white moves first
	
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

const MinMax& ReversiBoard::GetMinMax() const
{
	return mMinMax;
}

const std::pair<int, int> ReversiBoard::GetPlayerScores() const
{
	return { mDiscGrid.count(CELL_WHITE) ,  mDiscGrid.count(CELL_BLACK) };
}

int ReversiBoard::AvailableMoveCount() const
{
	const int m = std::count_if(mScoreGrid.begin(), mScoreGrid.end(), [](int n) 
		{
			return n >= MIN_SCORE;
		});

	return m;
}

std::vector<int> ReversiBoard::GetAvailableMoves() const
{
	std::vector<int> moves;
	moves.reserve(64);

	// add 1d locations of available moves
	for (size_t i = 0; i < mScoreGrid.size(); i++)
	{
		if (mScoreGrid.at(i) >= MIN_SCORE)
		{
			moves.push_back(i);
		}
	}

	return moves;
}

std::vector<int> ReversiBoard::GetMoveHistory() const
{
	std::vector<int> m;

	m.reserve(64);

	auto end = std::find(mMoveTracker.begin(), mMoveTracker.end(), -1);

	// copy on
	std::copy(mMoveTracker.begin(), end, std::back_inserter(m));

	return std::vector<int>();
}

void ReversiBoard::ExportWinningMoves()
{
	if ( !(CanMove() || mExportedWinningMoves) )
	{
		const auto finalScores(GetPlayerScores());

		// don't continue if a draw
		if (finalScores.first == finalScores.second)
		{
			return;
		}

		// copy of moves for manipulating
		auto moves = GetMoveHistory();

		std::ifstream inputMoveFile("Data/Payoffs/adaptive.csv",std::ios::in);

		if (inputMoveFile.is_open())
		{
			// invalid default value
			std::vector<int> tempMoveHistory(GRID_SIZE * GRID_SIZE, -1);

			// read values to temp vector
			int pos = 0;
			while (inputMoveFile.good() && pos < GRID_SIZE * GRID_SIZE)
			{
				std::string s;
				getline(inputMoveFile, s, ',');
				int i = std::atoi(s.c_str());
				tempMoveHistory.at(pos) = i;
				++pos;
			}
		
			inputMoveFile.close();


			//starts at location in move list
			size_t winner = (finalScores.first > finalScores.second)? 0 : 1; // white wins : black wins. draw already evaluated

			while (winner < moves.size())
			{
				++tempMoveHistory.at(moves.at(winner));
				// skip losing scores
				winner += 2; 
			}

			std::ofstream outputMovesFile("Data/Payoffs/adaptive.csv", std::ios::out);
			if (outputMovesFile.is_open())
			{
				// reset position
				pos = 0;

				// output moves
				while (pos < GRID_SIZE * GRID_SIZE)
				{
					outputMovesFile << tempMoveHistory.at(pos);

					if (++pos < GRID_SIZE * GRID_SIZE)
					{
						outputMovesFile << ',';
					}
				}
				outputMovesFile.close();
			}
			else
			{
				assert(false);
			}



		}
		else
		{
			assert(false);
		}

		// mark as exported
		mExportedWinningMoves = true;
	}


}
