
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

	// test board
	{
		ReversiBoard b;

		// game init
		b.Initialize();

		b.ToConsole();

		// y x
		std::array<Vector2i, 4> moves =
		{
			Vector2i{0,1},
			Vector2i{0,2},
			Vector2i{0,3},
			Vector2i{0,0}
		};



		while (b.CanMove())
		{
			ReversiBoard::MinMax minMax(b.GetMinMax());

			std::cout << "\n\nMove" << minMax.second.position.x << "," << minMax.second.position.y;

			b.Move(minMax.second.position);

			if (b.BoardHasChanged())
			{
				std::cout << "\n\nBoard changed\n\n\n";
				b.UpdateBoardBackup();
				// change visuals
			}
			else
			{
				std::cout << "\n\n\n\nBoard NOT changed\n\n\n\n\n";
			}

			b.ToConsole();
			system("Pause");
		}
		

		std::cout << "\n\n\n\nGAME OVER\n\n\n\n\n";

		
		system("Pause");


	}

}