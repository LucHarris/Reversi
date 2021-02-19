#pragma once
#include "Grid.h"

#ifndef PLAYER_ONE
#define PLAYER_ONE 0
#endif // !PLAYER_ONE

#ifndef PLAYER_TWO
#define PLAYER_TWO 1
#endif // !PLAYER_TWO

#ifndef ZERO_SCORE
#define ZERO_SCORE 0
#endif // !ZERO_SCORE

#ifndef GRID_SIZE
#define GRID_SIZE 8
#endif // !GRID_SIZE

#ifndef CELL_EMPTY
#define CELL_EMPTY '-'
#endif // !CELL_EMPTY

#ifndef CELL_WHITE
#define CELL_WHITE 'O'
#endif // !CELL_WHITE

#ifndef CELL_BLACK
#define CELL_BLACK 'T'
#endif // !CELL_BLACK

#ifndef MIN_SCORE
#define MIN_SCORE 2
#endif // !MIN_SCORE


// A board for square grid and 2 players
class Board
{
public:
	typedef Grid<char, GRID_SIZE, CELL_EMPTY,Vector2i>	DiscGrid;			// Player discs
	typedef std::array<DiscGrid, 2>				DiscGridBuffer;		// Defaults char to 0
	typedef Grid<int, GRID_SIZE, ZERO_SCORE, Vector2i>	ScoreGrid;
private:
	// Track changes to grid
	DiscGrid mDiscGrid;
	DiscGrid mDiscGridBackup;
	// Each cell contains number of cells that can be flipped
	// Available moves are scores above 0.
	ScoreGrid mScoreGrid;
	//Active player for buffer index
	size_t mPlayerIndex = 0;
	//Oppenent to active player
	size_t mOpponentIndex = 1;
	bool mGameEnded = false;



	// Based on generated score grid
	bool CanMove();
	
	// Dependant on mScoreGird populated and upto date
	void PlaceMove(const Vector2i& v);

	// Inside grid
	bool WithinLimits(const Vector2i& v)
	{
		return
			v.x < GRID_SIZE&&
			v.x >= 0 &&
			v.y < GRID_SIZE&&
			v.y >= 0;
	}

public:
	// starting grid
	void PopulateStart();

	// For either active player
	void GenerateScoreGrid();

	// Places a move for player.
	// Required mScoreGrid to be generated for current player
	void Move(const Vector2i& v);
	// Oppenents go
	void SwapPlayers();

	bool ValidCellForMove(const Vector2i& v);
	
	// Post player swap Oppenent buffer to active player buffer
	// Call after Move and BoardChanged logic
	void UpdateBoardBackup();

	void ToConsole();

	// Compares with backup to see if the board has changed
	bool BoardHasChanged()
	{
		return mDiscGrid != mDiscGridBackup;
	}
};

