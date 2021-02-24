#pragma once
#include <assert.h>
#include <array>
#include <algorithm>
#include "Vector2i.h"
#include <string>
#include <iostream>

template <
	typename VALUE,					// elements of the 2D array
	size_t SIZE, 					// axis size
	VALUE DEFAULT_VALUE = 0,		// set default element values
	typename COORD = Vector2i		// Easier access elements in 2D array
>
// 2D square grid
class Grid
{
private:
	std::array<VALUE, SIZE * SIZE> mTable;
	size_t mDimention = SIZE;
public:
	// default value constructor
	Grid(VALUE v = DEFAULT_VALUE)
	{
		fill(v);
	}

	// copy constructor
	Grid(const Grid& grid)
		:
		mTable(grid.mTable)
	{

	}

	bool operator !=(const Grid& b) const
	{
		return mTable != b.mTable && mDimention != b.mDimention;
	}

	//at

	// value at coordinates
	VALUE& at(size_t x, size_t y)
	{
		return mTable.at(y * SIZE + x); 
	}

	VALUE& at(COORD v)
	{
		assert(
			v.x >= 0 && 			
			v.y >= 0 &&			
			v.x < SIZE &&			
			v.y < SIZE
		);

		return mTable.at(v.y * SIZE + v.x);
	}

	// cells in a row
	size_t dimension_size()
	{
		return SIZE;
	}

	// total number of cells
	size_t size()
	{
		return mTable.size();
	}

	void to_console()
	{
		int count = 0;
		
		for (VALUE& element : mTable)
		{
			std::cout << element << " ";

			// increment and check for EOL
			if (++count >= SIZE)
			{
				std::cout << "\n";
				count = 0;
			}
		}
	}

	void fill(VALUE v)
	{
		std::fill(mTable.begin(), mTable.end(), v);
	}

	int count(VALUE v) const
	{
		return std::count_if(mTable.begin(), mTable.end(), [&v](const VALUE& element)
			{
				return v == element;
			});
	}
};
