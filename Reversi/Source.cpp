
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
		Board b;

		// game init
		b.PopulateStart();
		b.GenerateScoreGrid();
		b.ToConsole();
		//move player 1
		//Vector2i move{ 0,1 };
		//b.Move(move); // with player switch
		//b.ToConsole();
		//if (b.BoardChanged())
		//{
		//	std::cout << "\n\n\n\nBoard changed\n\n\n\n\n";
		//	// change visuals
		//	b.PostUpdateBuffer();
		//}

		//b.ToConsole();
		//system("Pause");


		// y x
		std::array<Vector2i, 4> moves =
		{
			Vector2i{0,1},
			Vector2i{0,2},
			Vector2i{0,3},
			Vector2i{0,0}
		};


		for (auto& m : moves)
		{
			std::cout << "\n\nMove" << m.x << "," << m.y;
			b.Move(m);
			if (b.BoardHasChanged())
			{
				std::cout << "\n\nBoard changed\n\n\n";
				// change visuals
				b.UpdateBoardBackup();
			}
			else
			{
				std::cout << "\n\n\n\nBoard NOT changed\n\n\n\n\n";
			}

			b.ToConsole();
			system("Pause");
		}
	}

}