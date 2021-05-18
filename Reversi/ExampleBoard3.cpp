// testing environment
#include "Board.h"
#include "NormalForm.h"
#include <chrono>
#include "PayoffMultipliers.h"
#include "PlayerManager.h"
#include <fstream>

const std::string 
	p1 = "PRE", 
	p2 = "EVE";

const std::string file = "Data/Out/" + std::string(p1) + " " + std::string(p2) + ".csv";

void Run()
{
	srand(time(NULL));
	ReversiBoard b;
	// game init
	b.Initialize();
	PlayerManager playerManager;
	playerManager.AddPlayer(Player::Type::HUMAN, 0);
	playerManager.AddPlayer(Player::Type::HUMAN, 1);
	NormalForm ai[2];
	PayoffMultipliers pom;
	pom.Load();


	ai[0].Init(pom.GetAt(PayoffMultipliers::PREDEFINED), pom.GetAt(PayoffMultipliers::ADAPTIVE));
	ai[1].Init(pom.GetAt(PayoffMultipliers::EVEN), pom.GetAt(PayoffMultipliers::ADAPTIVE));

	// player turns until win/draw
	while (b.CanMove())
	{
		// auto game for both players


		const int move = ai[playerManager.GetSide()].Evalualate(playerManager.GetSide(), b, b.GetLastMove());

		

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


		playerManager.Increment();
	}

	// Final score
	{

		std::ofstream ofs(file, std::ios::app);

		auto scores = b.GetPlayerScores();

		// white win = 0, black win = 1, draw = -1
		int winner = (scores.first > scores.second) ? 0: 1;
		if (scores.first == scores.second)
		{
			winner = -1;
		}

		ofs << winner << ',' << scores.first - scores.second << "\n";
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