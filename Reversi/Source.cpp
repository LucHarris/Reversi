
#include "Grid.h"
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

		int i = 0;
	}

}