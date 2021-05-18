// testing environment
#include "Board.h"
#include <chrono>

void Run()
{
	srand(time(NULL));

	ReversiBoard b;

	// game init
	b.Initialize();

	// player turns until win/draw
	while (b.CanMove())
	{
		// auto game for both players

		auto moves = b.GetAvailableMoves();

		int move = -1;
	
		if (moves.size())
		{
			move = moves.at(rand() % moves.size());
		}
		else
		{
			// can move but no moves?
			assert(false);
		}

		if (!b.Move(move))
		{
			assert(false);
		}

		if (b.BoardHasChanged())
		{
			// change visuals here
			b.UpdateBoardBackup();
		}
		else
		{
		}

	}

	// Final score
	{
		b.ExportWinningMoves();
	}

}

int main()
{
	int count = 5000;

	for (int i = 0; i < count; i++)
	{
		system("CLS");
		std::cout << (i * 100) / count << "% " << i << " of " << count;
		Run();
	}
}