#include "NormalForm.h"
#include "Constants.h"
#include <numeric>
#include <iostream>

NormalForm::NormalForm(const std::array<int,64>& payoffMulti)
	:
	mPayoffMultiplier(payoffMulti)
{
	

}

int NormalForm::Evalualate(int agent, ReversiBoard board, int opponentMove)
{
	// set up for new round
	entry.clear();
	pureNashEqui.clear();
	dominantStrategy.clear();
	mOpponentMove = opponentMove;

	// generates normal form based on board scores

	ScoreGrid scoreGrid = board.GetScoreGrid();

	// level 1
	if (board.CanMove())
	{
		for (int i = 0; i < (int)scoreGrid.size(); i++)
		{
			if (scoreGrid.at(i) >= MIN_SCORE)
			{
				ReversiBoard b1 = board;
				b1.Move(i);

				// level 2

				ScoreGrid sg2 = b1.GetScoreGrid();

				if (b1.CanMove())
				{
					for (int j = 0; j < (int)sg2.size(); j++)
					{
						if (sg2.at(j) >= MIN_SCORE)
						{
							ReversiBoard b2 = b1;
							b2.Move(j);

							// todo get score table and apply modifiers and accumulate for black/white

							{
								int   po1 = 0, po2 = 0;
								GeneratePayoffs(b2, po1, po2);

								// swap values if playing for black
								if (b2.GetActivePlayerDisc() == CELL_BLACK)
								{
									std::swap(po1, po2);
								}

								entry.push_back({ {i,j},{po1,po2},{-1,-1} });
							}
						}
					}
				}
				else
				{
					entry.push_back({ {i,-1},{100,0},{-1,-1} });
				}
			}
		}
	}
	else
	{
		entry.push_back({ {-1,-1},{0,100},{-1,-1} });
	}

	std::cout << "\n--start---------------------";
	std::cout << "\nAll entries for agent: " << agent;

	ToConsole();

	CalcPureNashMax();
	CalculateDominantMax();
	GeneratePureNashEquilib();
	GenerateDominant();

	bool actDominant = 1;

	// valid
	if (previousMove.key[0] >= 0 && mOpponentMove >= 0)
	{
		// opponent cooperated and available nash
		if (pureNashEqui.size() > 0 && mOpponentMove == previousMove.key[1])
		{
			actDominant = 0;
		}
	}

	int move = -1;

	if (actDominant)
	{
		int m = dominantStrategy.at(rand() % dominantStrategy.size()).key[0];
		move = m;
	}
	else
	{
		int m = pureNashEqui.at(rand() % pureNashEqui.size()).key[0];
		move = m;
	}


	assert(move >= 0);
	return move;
}

void NormalForm::ToConsole()
{
	std::cout << "\nall payoff";
	for (auto& e : entry)
	{
		e.ToConsole();
	}

	std::cout << "\npure nash";
	for (auto& e : pureNashEqui)
	{
		e.ToConsole();
	}
	std::cout << "\ndom";

	for (auto& e : dominantStrategy)
	{
		e.ToConsole();
	}
}

void NormalForm::GenerateDominant()
{
	dominantStrategy = entry;


	int p = 0;
	bool dominant[2]{ false,false };

	while (!(dominant[0] || dominant[1]) && dominantStrategy.size() > 0)
	{
		int player = p % 2;

		auto minmax = std::minmax_element(dominantStrategy.begin(), dominantStrategy.end(), [player](const Entry& lhs, const Entry& rhs)
			{
				return lhs.max[player] < rhs.max[player];
			});

		if (minmax.first->max[player] != minmax.second->max[player])
		{
			dominantStrategy.erase(std::remove_if(dominantStrategy.begin(), dominantStrategy.end(), [player, &minmax](const Entry& e)
				{
					return e.max[player] == minmax.first->max[player];
				}), dominantStrategy.end());
		}
		else
		{
			//break condition
			dominant[player] = true;
		}

		++p;
	}
}

int NormalForm::Dominant()

{

	std::cout << "\nDom start";
	//todo delete
	{
		auto temp = entry;

		std::cout << "\n  eg\n";

		entry.clear();
		entry.push_back({ {1,3},{30,10},{-1,-1} });
		entry.push_back({ {2,3},{20,20},{-1,-1} });

		entry.push_back({ {1,4},{00,00},{-1,-1} });
		entry.push_back({ {2,4},{20,20},{-1,-1} });

		//entry.push_back({ {1,5},{50,40},{-1,-1} });
		//entry.push_back({ {2,5},{40,50},{-1,-1} });

		CalcPureNashMax();
		CalculateDominantMax();
		GeneratePureNashEquilib();
		GenerateDominant();

		//CalculateDominantMax();
		//EliminateActions();

		ToConsole();
		std::cout << "\n";
		entry = temp;
	}



	CalculateDominantMax();
	std::cout << "\n";
	ToConsole();
	std::cout << "\n";
	EliminateActions();
	std::cout << "\n";
	std::cout << "\nRemaining entries";
	ToConsole();


	

	// todo remove logic
	// true chooses max payoff[0]
	// false randomly selects from remaining options for variety
	if (false)
	{
		// decide on remaining actions
		auto ret = std::max_element(entry.begin(), entry.end(), [](const Entry& lhs, const Entry& rhs)
			{
				return lhs.payoff[0] < rhs.payoff[0];
			});

		std::cout << "\nReturn entry";
		if (ret != entry.end())
		{
			ret->ToConsole();
		}
		else
		{
			std::cout << "\nNONE returning -1";
		}

		std::cout << "\n--end---------------------";
		std::cout << "\ndom end";

		return (ret != entry.end()) ? ret->key[0] : -1;
	}
	else
	{
		std::cout << "\ndom end with rand!!!";

		return (entry.size() > 0) ? entry.at(rand() % entry.size()).key[0] : -1;
	}


}

void NormalForm::CalculateDominantMax()
{
	// Get max for each action for players
	for (int p = 2; p > 0; --p)
	{
		const int player = p % 2;
		std::sort(entry.begin(), entry.end(), [player](const Entry& lhs, const Entry& rhs)
			{
				return lhs.key[player] < rhs.key[player];
			});

		// first of action a
		std::vector<Entry>::iterator first = entry.begin();
		// last of action a
		std::vector<Entry>::iterator last = first;
		// find key for player
		std::vector <Entry> find(1);

		while (first != entry.end() && last != entry.end())
		{
			// set action to find
			find[0] = *first;

			// find last entry
			last = std::find_end(entry.begin(), entry.end(), find.begin(), find.end(), [player](const Entry& lhs, const Entry& rhs)
				{
					return lhs.key[player] == rhs.key[player];
				});

			// set max for each action
			if (last != entry.end())
			{
				std::vector<Entry>::iterator max = std::max_element(first, last, [player](const Entry& lhs, const Entry& rhs)
					{
						return lhs.payoff[player] < rhs.payoff[player];
					});

				for (auto& e : entry)
				{
					if (e.key[player] == max->key[player])
					{
						e.max[player] = max->payoff[player];
					}
				}
			}

			first = last;

			++first;

		}
	}

	// sorted by second player key 
	// reset sorting for debugging
	// todo remove 
	std::sort(entry.begin(), entry.end(), [](const Entry& lhs, const Entry& rhs)
		{
			return lhs.key[0] < rhs.key[0];
		});
}

void NormalForm::CalcPureNashMax()
{
	// Get max for each action for players
	for (int p = 2; p > 0; --p)
	{
		const int player = p % 2; // 2 -> 0 && 1 -> 1
		const int sortByPlayer = 1 - player;
		std::sort(entry.begin(), entry.end(), [sortByPlayer](const Entry& lhs, const Entry& rhs)
			{
				return lhs.key[sortByPlayer] < rhs.key[sortByPlayer];
			});

		// first of action a
		std::vector<Entry>::iterator first = entry.begin();
		// last of action a
		std::vector<Entry>::iterator last = first;
		// find key for player
		std::vector <Entry> find(1);

		// every opponent strategy
		while (first != entry.end() && last != entry.end())
		{
			// set action to find
			find[0] = *first;

			// find last entry
			last = std::find_end(entry.begin(), entry.end(), find.begin(), find.end(), [sortByPlayer](const Entry& lhs, const Entry& rhs)
				{
					return lhs.key[sortByPlayer] == rhs.key[sortByPlayer];
				});

			// set max for each action
			if (last != entry.end())
			{
				++last;
				std::vector<Entry>::iterator max = std::max_element(first, last, [player](const Entry& lhs, const Entry& rhs)
					{
						return lhs.payoff[player] < rhs.payoff[player];
					});

				std::transform(first, last, first, [&max,&player](Entry& e) 
					{
						e.nash[player] = max->payoff[player];

						return e;
					});
			}

			first = last;

			//++first;

		}
	}

	// sorted by second player key 
	// reset sorting for debugging
	// todo remove 
	std::sort(entry.begin(), entry.end(), [](const Entry& lhs, const Entry& rhs)
		{
			return lhs.key[0] < rhs.key[0];
		});
}

void NormalForm::EliminateActions()
{
	
}

void NormalForm::GeneratePayoffs(const ReversiBoard& board, int& p1, int& p2)
{
	const auto& b = board.GetDiscGrid();

	Grid<int, gc::BOARD_DISC_ROW_SIZE> payoffBoard(0);

	for (int i = 0; i < gc::BOARD_DISC_SIZE; i++)
	{
		// defaults if tile isn't occuied by a disc
		int disc = 0;
		switch (b.at(i))
		{
		case CELL_WHITE: disc = 1;
			break;
		case CELL_BLACK: disc = -1;
			break;
		}

		payoffBoard.at(i) = disc;
	}

	// applies multiplier
	std::transform(
		mPayoffMultiplier.begin(),
		mPayoffMultiplier.end(),
		payoffBoard.begin(),
		payoffBoard.begin(),
		std::multiplies<int>()
	);

	/*std::transform(
		gc::CORNER_SCORE_MULTPLIER,
		gc::CORNER_SCORE_MULTPLIER + gc::BOARD_DISC_SIZE,
		payoffBoard.begin(),
		payoffBoard.begin(),
		std::multiplies<int>()
	);*/

	// p1 payoff sums the positive values for a positive payoff
	p1 = std::accumulate(payoffBoard.begin(), payoffBoard.end(),0, [](int a, int b) 
		{
			if (b > 0)
			{
				return a + b;
			}
			else
			{
				return a;
			}

		});

	// p2 payoff converts minus values to a positive payoff
	p2 = std::accumulate(payoffBoard.begin(), payoffBoard.end(), 0, [](int a, int b)
		{
			if (b < 0)
			{
				return a - b;
			}
			else
			{
				return a;
			}

		});

}

void NormalForm::GeneratePureNashEquilib()
{
	std::copy_if(entry.begin(), entry.end(), std::back_inserter(pureNashEqui), &Entry::IsPureNash);

}

void Entry::ToConsole() const
{
	std::cout
		<< "\nkey[ " << key[0] << " , " << key[1] << " ]"
		<< " payoff[ " << payoff[0] << " , " << payoff[1] << " ]"
		<< " max[ " << max[0] << " , " << max[1] << " ]" 
		<< " nask[ " << nash[0] << " , " << nash[1] << " ]";

}

bool Entry::IsPureNash(const Entry& e)
{
	const bool valid =
		e.payoff[0] >= 0 &&
		e.payoff[1] >= 0 &&
		e.nash[0] >= 0 &&
		e.nash[1] >= 0;

	const bool isNash =
		e.payoff[0] == e.nash[0] &&
		e.payoff[1] == e.nash[1];


	return valid && isNash;
}

//bool Entry::IsPureNash() const
//{
//	const bool valid =
//		key[0] >= 0 &&
//		key[1] >= 0 &&
//		nash[0] >= 0 &&
//		nash[1] >= 0;
//
//	const bool isNash =
//		key[0] == nash[0] &&
//		key[1] == nash[1];
//
//
//	return valid && isNash;
//}
