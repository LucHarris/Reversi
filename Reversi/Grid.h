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
public:
	typedef typename std::array<VALUE, SIZE> Row;
	typedef typename std::array<Row, SIZE> Table;
private:
	Table mTable;

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
		bool notEqual = false;

		Vector2i pos;
		while (pos.x < SIZE && !notEqual)
		{
			while (pos.y < SIZE && !notEqual)
			{

				VALUE aa = mTable.at(pos.x).at(pos.y);
				VALUE bb = b.mTable.at(pos.x).at(pos.y);
				if (aa != bb)
				{
					notEqual = true;
				}
				++pos.y;
			}
			++pos.x;
		}

		return notEqual;
	}

	//at

	// value at coordinates
	VALUE& at(size_t x, size_t y)
	{
		return mTable.at(x).at(y);
	}

	VALUE& at(Vector2i v)
	{
		assert(v.x >= 0 && v.y >= 0);
		return mTable.at(v.x).at(v.y);
	}

	// cells in a row
	size_t row_size()
	{
		return mTable.at(0).size();
	}

	// total number of cells
	size_t size()
	{
		return mTable.size() * row_size();
	}

	void to_console()
	{
		for (auto& row : mTable)
		{
			for (VALUE& cell : row)
			{
				std::cout << cell << " ";
			}
			std::cout << "\n";
		}
	}

	void fill(VALUE v)
	{
		// fill each row with default value
		std::for_each(mTable.begin(), mTable.end(), [&v](Row& r) 
			{
			std::fill(r.begin(), r.end(), v);
			});
	}
};
