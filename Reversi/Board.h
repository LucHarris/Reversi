#pragma once
#include "Grid.h"

#ifndef PLAYER_ACTIVE
#define PLAYER_ACTIVE 0
#endif // !PLAYER_ACTIVE

#ifndef PLAYER_OPPONENT
#define PLAYER_OPPONENT 1
#endif // !PLAYER_OPPONENT

#ifndef ZERO_SCORE
#define ZERO_SCORE 0
#endif // !ZERO_SCORE

#ifndef GRID_SIZE
#define GRID_SIZE 8
#endif // !GRID_SIZE

#ifndef CELL_EMPTY
#define CELL_EMPTY ' '
#endif // !CELL_EMPTY

#ifndef CELL_WHITE
#define CELL_WHITE 'X'
#endif // !CELL_WHITE

#ifndef CELL_BLACK
#define CELL_BLACK 'O'
#endif // !CELL_BLACK

// A board for square grid and 2 players
class Board
{
public:
	typedef Grid<char, GRID_SIZE, CELL_EMPTY,Vector2i>	DiscGrid;			// Player discs
	typedef std::array<DiscGrid, 2>				DiscGridBuffer;		// Defaults char to 0
	typedef Grid<int, GRID_SIZE, ZERO_SCORE, Vector2i>	ScoreGrid;
private:
	// Track changes to grid
	DiscGridBuffer mDiscGridBuffer;

	// Each cell contains number of cells that can be flipped
	// Available moves are scores above 0.
	ScoreGrid mScoreGrid;
	//Active player
	size_t mPlayerIndex = 0;
	//Oppenent to active player
	size_t mOpponentIndex = 1;
	bool mGameActive = true;
public:

	// Oppenents go
	void SwapPlayers();

	// For either active player or opponent
	void GenerateScoreGrid(size_t playerIndex);

	// For either active player or opponent
	bool PlayerCanMove(size_t playerIndex);

	// Places a move for player.
	// Required mScoreGrid to be generated for current player
	void Move(const Vector2i& v)
	{
		if (ValidCellForMove(v))
		{
			PlaceMove(v);
			

		}
	}

	bool ValidCellForMove(const Vector2i& v)
	{
		return mScoreGrid.at(v) > ZERO_SCORE;
	}

	// Based on generated score grid
	bool CanMove()
	{
		bool canMove = false;

		Vector2i v { 0,0 };
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
	}


	void PlaceMove(const Vector2i& v)
	{
		Vector2i offset = v;
		int count = 0;
		const char Opponent = (mGameActive == 0) ? CELL_WHITE : CELL_BLACK;
		const char Active = (mGameActive == 0) ? CELL_BLACK : CELL_WHITE;

		for(auto& direction : Vector2i::Directions)
		{
			// reset count
			count = 0;
			// first step in direction (reset)
			offset = v + direction; 
			// within limits and next cell in direction is oppenents disc
			while (WithinLimits(offset) && mDiscGridBuffer.at(mGameActive).at(offset) == Opponent)
			{
				++count; 
				offset += direction;
			}

			// valid move
			if (mDiscGridBuffer.at(mPlayerIndex).at(v) == Active )
			{
				// travel back to v changing discs
				for (; count > 0; --count)
				{
					offset -= direction;
					mDiscGridBuffer.at(mPlayerIndex).at(offset) = Active;
				}
			}
		}

		/*
				for each direction from x,y (xd,yd)
				count = 0
				While (next step in range and is OPPONENT)
				{
					++count
					nextstep += direction
				}
				if(next step is PLAYER)
					for( x,y;    x,y != next step ; x,y += xd,dy)
						table at x,y = OPPONENT
		*/

		//mDiscGridBuffer.at(PLAYER_ACTIVE).at(v);

	}

	bool WithinLimits(const Vector2i& v)
	{
		return
			v.x < GRID_SIZE&&
			v.x >= 0 &&
			v.y < GRID_SIZE&&
			v.y >= 0;
	}
	// Oppenent buffer to active player buffer
	void UpdateBuffer()
	{
		// override old buffer
		mDiscGridBuffer.at(mOpponentIndex) = mDiscGridBuffer.at(mPlayerIndex);
	}
};

