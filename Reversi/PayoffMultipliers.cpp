#include "PayoffMultipliers.h"
#include <cassert>
#include <fstream>

PayoffMultipliers::PayoffMultipliers()
	:
	mPaths
	{
		{"Data/Payoffs/even.csv"},
		{"Data/Payoffs/predefined.csv"},
		{"Data/Payoffs/adaptive.csv"}
	}
{

}

void PayoffMultipliers::Load()
{
	for (size_t i = 0; i < COUNT; i++)
	{
		std::ifstream inputMoveFile(mPaths[i], std::ios::in);

		if (inputMoveFile.is_open())
		{
			// read values to payoff array
			int pos = 0;
			while (inputMoveFile.good() && pos < 64)
			{
				std::string s;
				getline(inputMoveFile, s, ',');
				int p = std::atoi(s.c_str());
				mPayoffMultipliers.at(i).at(pos) = p;
				++pos;
			}

			inputMoveFile.close();
		}
		else
		{
			assert(false);
		}
	}


	
}


const std::array<int, 64>& PayoffMultipliers::GetAt(size_t i) const 
{
	assert(i < mPayoffMultipliers.size());
	return mPayoffMultipliers.at(i);
}
