#include "NormalForm.h"
#include "Constants.h"
#include <numeric>
#include <iostream>
#include <cassert>

NormalForm::NormalForm()
{
}

void NormalForm::Init(const std::array<int, 64>& payoffMulti, const ScoreGrid& prob)
{

	for (size_t i = 0; i < payoffMulti.size(); i++)
	{
		mPayoffMultiplier.at(i) = payoffMulti.at(i);
	}

	mProb = prob;

	// validate 
	auto min = std::min_element(mProb.begin(), mProb.end());
	assert(*min > 0);

	dominantStrategy.reserve(64);
	pureNashEqui.reserve(64);
	entry.reserve(64);
}

int NormalForm::Evalualate(int agent, ReversiBoard board, int opponentMove)
{
	// set up for new round
	entry.clear();
	pureNashEqui.clear();
	dominantStrategy.clear();
	mExpectedUtilTotal[0].clear();
	mExpectedUtilTotal[1].clear();

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

	//std::cout << "\n--start---------------------";
	//std::cout << "\nAll entries for agent: " << agent;


	ToConsole();
	bool actDominant = true;

	// valid
	if (!previousMove.empty())
	{
		// looks previous game
		auto found = std::find_if(previousMove.begin(), previousMove.end(), [&](const Entry& e) 
			{
				return
					mOpponentMove < 0 || // no previous move
					e.key[1] == mOpponentMove // cooperated
					;
			});

		if (found != previousMove.end() )
		{
			if (Entry::IsPureNash(*found) || mOpponentMove < 0)
			{
				std::cout
					<< "\n------------"
					<< "\n------------"
					<< "\n------------"
					<< "\n choice to coop"
					<< "\n------------"
					<< "\n------------"
					<< "\n------------"
					;

				actDominant = false;
			}
		}
		else
		{
			std::cout << "Cant find move";
			assert(false);
		}
	}

	CalculateDominantMax();
	CalcPureNashMax();

	int move = -1;

	if (actDominant)
	{
		GenerateDominant();

		if (!dominantStrategy.empty())
		{
			move = GetMixedMove(dominantStrategy);
		}
		else // empty or multiple 
		{
			move = GetMixedMove(entry);
		}
	}
	else
	{
		GeneratePureNashEquilib();

		if (!pureNashEqui.empty())
		{
			move = GetMixedMove(pureNashEqui);
		}
		else // empty or multiple 
		{
			move = GetMixedMove(entry);
		}
	}


	assert(move >= 0);

	previousMove.clear();
	std::copy_if(entry.begin(), entry.end(), std::back_inserter(previousMove), [&move](const Entry& e) 
		{
			return e.key[0] == move;
		});


	return move;
}

void NormalForm::ToConsole()
{
	/*std::cout << "\nall payoff";
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
	}*/
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
			// best response first player
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

std::pair<float, float> NormalForm::CalcMixedNash(std::vector<Entry>& entries)
{

	std::pair<float, float> prob = std::make_pair(-1.0f, -1.0f);

	if (entries.size() > 0)
	{
		if (entries.at(0).key[1] >= 0)
		{
			// generate expected utility
			for (auto& e : entries)
			{
				// totals
				if (e.key[1] >= 0)
				{
					mExpectedUtilTotal[0][e.key[0]] += mProb.at(e.key[1]);
				}

				if (e.key[1] >= 0)
				{
					mExpectedUtilTotal[1][e.key[1]] += mProb.at(e.key[0]);
				}
			}

			for (auto& e : entries)
			{
				if (e.key[0] >= 0)
				{
					e.expUtil[0] = (float)mProb.at(e.key[0]) / (float)mExpectedUtilTotal[0][e.key[0]];

				}
				else
				{
					e.expUtil[0] = 0;
				}

				if (e.key[1] >= 0)
				{
					e.expUtil[1] = (float)mProb.at(e.key[1]) / (float)mExpectedUtilTotal[1][e.key[1]];
				}
				else
				{
					e.expUtil[1] = 0;
				}
			}
			float sum = 0.0f;
			

			// sums of all posibilities equals 1.0f
			for (auto& e : entries)
			{
				sum += e.expUtil[0] * e.expUtil[1];
			}

			std::cout << "\nMixed validation = " << sum;

			float chancePlayOptim[2]{ 0,0 };

			prob.first  = 0.0f;
			prob.second = 0.0f;

			for (auto& e : entries)
			{
				prob.first += e.payoff[0] * e.expUtil[0];
				prob.second += e.payoff[1] * e.expUtil[1];

			}

			prob.first = chancePlayOptim[0];
			prob.second = chancePlayOptim[1];

			std::cout << "\n mixed [0] = " << prob.first << " [1] = " << prob.second;
		}
	}

	return prob;

}

int NormalForm::GetMixedMove(std::vector<Entry>& entries)
{

	int maxPlayer = 0;
	auto p = CalcMixedNash(entries);

	if (p.second > p.first)
	{
		maxPlayer = 1;
	}

	// action with most likely chance of getting picked
	auto move = std::max_element(entries.begin(), entries.end(), [maxPlayer](const Entry& e, const Entry& f)
		{
			return e.payoff[maxPlayer] < f.payoff[maxPlayer];
		});

	std::vector<Entry> maxEntries;
	
	// all emements with max payoff
	std::copy_if(entries.begin(), entries.end(), std::back_inserter(maxEntries), [&](const Entry& e) 
		{
			return e.payoff[maxPlayer] >= move->payoff[maxPlayer];
		});

	// choose from any element with max payoff for variation
	if (!maxEntries.empty())
	{
		return maxEntries.at(rand() % maxEntries.size()).key[0];
	}
	else
	{
		return move->key[0];
		assert(false);
	}
}

void NormalForm::Reset()
{
	mOpponentMove = -1;
}

void NormalForm::Test()
{
	NormalForm nf = *this;
	
	entry.clear();
	entry.push_back({ {1,3},{330,10},{-1,-1} });
	entry.push_back({ {2,3},{20,20},{-1,-1} });

	entry.push_back({ {1,4},{00,00},{-1,-1} });
	entry.push_back({ {2,4},{20,20},{-1,-1} });

	//entry.push_back({ {1,5},{50,40},{-1,-1} });
	//entry.push_back({ {2,5},{40,50},{-1,-1} });

	CalcPureNashMax();
	CalculateDominantMax();
	GeneratePureNashEquilib();
	GenerateDominant();
	//CalcMixedNash();
	std::cout << "\n-------test start";
	ToConsole();
	std::cout << "\n-------test end";


	
	*this = nf;
}

void Entry::ToConsole() const
{
	std::cout
		<< "\nkey[ " << key[0] << " , " << key[1] << " ]"
		<< " payoff[ " << payoff[0] << " , " << payoff[1] << " ]"
		<< " max[ " << max[0] << " , " << max[1] << " ]"
		<< " nash[ " << nash[0] << " , " << nash[1] << " ]"
		<< " equi[ " << expUtil[0] << " , " << expUtil[1] << " ]"
		;
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