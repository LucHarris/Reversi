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

	// validation
	auto min = std::min_element(mProb.begin(), mProb.end());
	assert(*min > 0);
}

int NormalForm::Evalualate(int agent, ReversiBoard board, int opponentMove)
{
	// set up for new round
	mAllStrategies.clear();
	mPureNash.clear();
	mDominantStrategies.clear();
	mExpectedUtilTotal[0].clear();
	mExpectedUtilTotal[1].clear();

	mOpponentMove = opponentMove;

	// generates normal form based on board scores

	ScoreGrid scoreGrid = board.GetScoreGrid();

	// simulates all possibilities in the game
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

								mAllStrategies.push_back({ {i,j},{po1,po2},{-1,-1} });
							}
						}
					}
				}
				else
				{
					mAllStrategies.push_back({ {i,-1},{100,0},{-1,-1} });
				}
			}
		}
	}
	else
	{
		// default invalid entry
		mAllStrategies.push_back({ {-1,-1},{0,100},{-1,-1} });
	}

	bool actDominant = true;

	// valid
	if (!mPreviousMove.empty())
	{
		// looks for previous game
		auto found = std::find_if(mPreviousMove.begin(), mPreviousMove.end(), [&](const Entry& e) 
			{
				return
					mOpponentMove < 0 || // no previous move
					e.key[1] == mOpponentMove // cooperated
					;
			});


		// decide if cooperative
		if (found != mPreviousMove.end() )
		{
			if (Entry::IsPureNash(*found) || mOpponentMove < 0)
			{
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

	// if unable to determine a single course of action evaluate with mixed strategy
	if (actDominant)
	{
		GenerateDominant();

		if (!mDominantStrategies.empty())
		{
			move = GetMixedMove(mDominantStrategies);
		}
		else // empty or multiple 
		{
			move = GetMixedMove(mAllStrategies);
		}
	}
	else
	{
		GeneratePureNashEquilib();

		if (!mPureNash.empty())
		{
			move = GetMixedMove(mPureNash);
		}
		else // empty or multiple 
		{
			move = GetMixedMove(mAllStrategies);
		}
	}


	assert(move >= 0);

	// updates previous move for next round
	mPreviousMove.clear();
	std::copy_if(mAllStrategies.begin(), mAllStrategies.end(), std::back_inserter(mPreviousMove), [&move](const Entry& e) 
		{
			return e.key[0] == move;
		});


	return move;
}

void NormalForm::ToConsole()
{
}

void NormalForm::GenerateDominant()
{
	mDominantStrategies = mAllStrategies;

	int p = 0;
	bool dominant[2]{ false,false };

	// elimiante strategies
	while (!(dominant[0] || dominant[1]) && mDominantStrategies.size() > 0)
	{
		int player = p % 2;

		auto minmax = std::minmax_element(mDominantStrategies.begin(), mDominantStrategies.end(), [player](const Entry& lhs, const Entry& rhs)
			{
				return lhs.max[player] < rhs.max[player];
			});

		if (minmax.first->max[player] != minmax.second->max[player])
		{
			mDominantStrategies.erase(std::remove_if(mDominantStrategies.begin(), mDominantStrategies.end(), [player, &minmax](const Entry& e)
				{
					return e.max[player] == minmax.first->max[player];
				}), mDominantStrategies.end());
		}
		else
		{
			//break condition
			dominant[player] = true;
		}

		++p;
	}
}

void NormalForm::CalculateDominantMax()
{
	// Get max for each action for players
	for (int p = 2; p > 0; --p)
	{
		const int player = p % 2;
		std::sort(mAllStrategies.begin(), mAllStrategies.end(), [player](const Entry& lhs, const Entry& rhs)
			{
				return lhs.key[player] < rhs.key[player];
			});

		// first of action a
		std::vector<Entry>::iterator first = mAllStrategies.begin();
		// last of action a
		std::vector<Entry>::iterator last = first;
		// find key for player
		std::vector <Entry> find(1);

		while (first != mAllStrategies.end() && last != mAllStrategies.end())
		{
			// set action to find
			find[0] = *first;

			// find last mAllStrategies
			last = std::find_end(mAllStrategies.begin(), mAllStrategies.end(), find.begin(), find.end(), [player](const Entry& lhs, const Entry& rhs)
				{
					return lhs.key[player] == rhs.key[player];
				});

			// set max for each action
			if (last != mAllStrategies.end())
			{
				std::vector<Entry>::iterator max = std::max_element(first, last, [player](const Entry& lhs, const Entry& rhs)
					{
						return lhs.payoff[player] < rhs.payoff[player];
					});

				for (auto& e : mAllStrategies)
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
	std::sort(mAllStrategies.begin(), mAllStrategies.end(), [](const Entry& lhs, const Entry& rhs)
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
		std::sort(mAllStrategies.begin(), mAllStrategies.end(), [sortByPlayer](const Entry& lhs, const Entry& rhs)
			{
				return lhs.key[sortByPlayer] < rhs.key[sortByPlayer];
			});

		// first of action a
		std::vector<Entry>::iterator first = mAllStrategies.begin();
		// last of action a
		std::vector<Entry>::iterator last = first;
		// find key for player
		std::vector <Entry> find(1);

		// every opponent strategy
		while (first != mAllStrategies.end() && last != mAllStrategies.end())
		{
			// set action to find
			find[0] = *first;

			// find last mAllStrategies
			last = std::find_end(mAllStrategies.begin(), mAllStrategies.end(), find.begin(), find.end(), [sortByPlayer](const Entry& lhs, const Entry& rhs)
				{
					return lhs.key[sortByPlayer] == rhs.key[sortByPlayer];
				});

			// set max for each action
			// best response first player
			if (last != mAllStrategies.end())
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
		}
	}

	// sorted by second player key 
	// reset sorting for debugging
	std::sort(mAllStrategies.begin(), mAllStrategies.end(), [](const Entry& lhs, const Entry& rhs)
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
	std::copy_if(mAllStrategies.begin(), mAllStrategies.end(), std::back_inserter(mPureNash), &Entry::IsPureNash);
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

			// change of action in strategies
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

			prob.first  = 0.0f;
			prob.second = 0.0f;

			for (auto& e : entries)
			{
				prob.first += e.payoff[0] * e.expUtil[0];
				prob.second += e.payoff[1] * e.expUtil[1];
			}
		}
	}

	return prob;
}

int NormalForm::GetMixedMove(std::vector<Entry>& entries)
{
	// player 1
	int maxPlayer = 0;
	auto p = CalcMixedNash(entries);

	// player 2
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