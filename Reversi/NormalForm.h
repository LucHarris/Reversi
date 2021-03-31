#pragma once
#include "Board.h"
#include <map>
#include <vector>

// move to cpp

typedef std::map<std::pair<int, int>, std::pair<int, int>> Matrix;


// tow player normal form payoffs
struct Entry
{
	// actions p1,p2
	int key[2]{ -1,-1 };
	// p1,p2
	int payoff[2]{ -1,-1 };
	// max for action
	int max[2]{ -1,-1 };
};



class NormalForm
{
	Matrix mMatrix;
	std::vector<Entry> entry;

public:
	/*NormalForm() 
	{

	};*/

	NormalForm(int agent, ReversiBoard board)
	{
		ScoreGrid scoreGrid = board.GetScoreGrid();

		// level 1
		if (board.CanMove())
		{
			for (int i = 0; i < scoreGrid.size(); i++)
			{
				if (scoreGrid.at(i) >= MIN_SCORE)
				{
					ReversiBoard b1 = board;
					b1.Move(i);

					// level 2

					ScoreGrid sg2 = b1.GetScoreGrid();

					if (b1.CanMove())
					{
						for (int j = 0; j < sg2.size(); j++)
						{
							if (sg2.at(j) >= MIN_SCORE)
							{
								ReversiBoard b2 = b1;
								b2.Move(j);

								// todo get score table and apply modifiers and accumulate for black/white

								// using scores as payout
								auto s = b2.GetPlayerScores();
								mMatrix[{i, j}] = s;

								entry.push_back({ {i,j},{s.first,s.second},{-1,-1} });

							}
						}
					}
				}
			}
		}
	}




	// @return evaluated move. -1 for invalid option
	int Dominant()
	{
        CalculateMax();

        EliminateActions();

        // decide on remaining actions
        auto ret = std::max_element(entry.begin(), entry.end(), [](const Entry& lhs, const Entry& rhs)
            {
                return lhs.payoff[0] < rhs.payoff[0];
            });


        
        return (ret != entry.end()) ? ret->key[0] : -1;
	}


private:
    void CalculateMax()
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
        // reset sorting
        // todo remove 
        std::sort(entry.begin(), entry.end(), [](const Entry& lhs, const Entry& rhs)
            {
                return lhs.key[0] < rhs.key[0];
            });
    }

    void EliminateActions()
    {
        int p = 0;
        bool dominant[2]{ false,false };

        while (!(dominant[0] || dominant[1]))
        {
            int player = p % 2;

            auto minmax = std::minmax_element(entry.begin(), entry.end(), [player](const Entry& lhs, const Entry& rhs)
                {
                    return lhs.max[player] < rhs.max[player];
                });

            if (minmax.first->max[player] != minmax.second->max[player])
            {
                entry.erase(std::remove_if(entry.begin(), entry.end(), [player, &minmax](const Entry& e)
                    {
                        return e.max[player] == minmax.first->max[player];
                    }), entry.end());
            }
            else
            {
                //break condition
                dominant[player] = true;
            }

            ++p;
        }
    }

};