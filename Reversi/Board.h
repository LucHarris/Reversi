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

#ifndef MAX_SCORE
#define MAX_SCORE GRID_SIZE * GRID_SIZE
#endif // !MAX_SCORE


struct MoveData
{
	Vector2i position{-1,-1};		// Board position 
	int score = 0;					//ScoreGrid data 
	size_t player;					// player index
	// validates position and score
	const bool IsValid() const;
};

typedef Grid<char, GRID_SIZE,/* CELL_EMPTY,*/Vector2i>	DiscGrid;			// Player discs
typedef std::array<DiscGrid, 2>							DiscGridBuffer;		// Defaults char to 0
typedef Grid<int, GRID_SIZE, /*ZERO_SCORE,*/ Vector2i>	ScoreGrid;
typedef  std::pair<MoveData, MoveData>					MinMax;

// A board for square grid and 2 players
class ReversiBoard
{
public:
	
private:
	// Track changes to grid
	DiscGrid mDiscGrid;
	DiscGrid mDiscGridBackup;
	// Each cell contains number of cells that can be flipped
	// Available moves are scores above 0.
	ScoreGrid mScoreGrid;
	// first is min second is max. Updated in GenerateScoreGrid()
	MinMax mMinMax;
	//Active player for buffer index
	size_t mPlayerIndex = 0;
	//Oppenent to active player
	size_t mOpponentIndex = 1;

	// Dependant on mScoreGird populated and upto date
	// returns if move is placed
	bool PlaceMove(const Vector2i& v);

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
	// Sets up the board for the first move
	void Initialize();

	// starting grid
	void PopulateStart();

	// For either active player
	void GenerateScoreGrid();

	// Negtive winning condition based on generated score grid has a valid score 
	bool CanMove();

	// Places a move for player.
	// Required mScoreGrid to be generated for current player
	bool Move(const Vector2i& v);
	// Places a move for player.
	// Required mScoreGrid to be generated for current player

	bool Move(int a);

	// Oppenents go
	void SwapPlayers();
	bool ValidCellForMove(const Vector2i& v);
	bool ValidCellForMove(int a);
	// Post player swap Oppenent buffer to active player buffer
	// Call after Move and BoardChanged logic
	void UpdateBoardBackup();

	void ToConsole();

	// Compares with backup to see if the board has changed
	bool BoardHasChanged()
	{
		return mDiscGrid != mDiscGridBackup;
	}

	const char GetActivePlayerDisc() const;

	const char GetActiveOpponentDisc() const;

	const MinMax& GetMinMax() const;

	const std::pair<int, int> GetPlayerScores() const;

	DiscGrid& GetDiscGrid()
	{
		return mDiscGrid;
	}

	const DiscGrid& GetDiscGrid() const
	{
		return mDiscGrid;
	}

	ScoreGrid& GetScoreGrid()
	{
		return mScoreGrid;
	}

	size_t GetPlayerIndex()
	{
		return mPlayerIndex;
	}
};

