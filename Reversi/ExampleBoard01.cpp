
#include "Board.h"

int main()
{
	// test grid
	{
		Grid<char, 8> gameTable;
		gameTable.at(0, 0) = 7;

		Grid<char, 8> gameTable2('1');

		gameTable = gameTable2;
	}

	// test auto board
	{
		ReversiBoard b;

		// game init
		b.Initialize();

		b.ToConsole();

		// player turns until win/draw
		while (b.CanMove())
		{
			// auto game for both players
			ReversiBoard::MinMax minMax(b.GetMinMax());

			std::cout << "\n\nMove" << minMax.second.position.x << "," << minMax.second.position.y;

			b.Move(minMax.second.position);

			if (b.BoardHasChanged())
			{
				std::cout << "\n\nBoard changed\n\n\n";
				// change visuals here
				b.UpdateBoardBackup();
			}
			else
			{
				std::cout << "\n\n\n\nBoard NOT changed\n\n\n\n\n";
			}

			b.ToConsole();
			//system("Pause");
		}
		
		// Final score
		{
			auto scores = b.GetPlayerScores();

			std::string winner = (scores.first > scores.second) ? "One" : "Two";

			if ((scores.first == scores.second))
			{
				winner = "Draw";
			}

			std::cout
				<< "\n\n\n\nGAME OVER\n\n"
				<< "\nPlayer 1(O): "
				<< scores.first
				<< "\nPlayer 2(T): "
				<< scores.second
				<< "\n\nWinner: " << winner
				;

			system("Pause");
		}

		


	}

}