#include "Board.h"

void Board::SwapPlayers()
{
	std::swap(mPlayerIndex, mOpponentIndex);
}
