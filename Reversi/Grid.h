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
	//VALUE DEFAULT_VALUE = 0,		// set default element values
	typename COORD = Vector2i		// Easier access elements in 2D array
>
// 2D square grid
class Grid
{
	typedef typename std::array<VALUE,SIZE * SIZE>::iterator				iterator;
	typedef typename std::array<VALUE,SIZE * SIZE>::const_iterator			const_iterator;
	typedef typename std::array<VALUE,SIZE * SIZE>::size_type				size_type;
	typedef typename std::array<VALUE,SIZE * SIZE>::reference				reference;
	typedef typename std::array<VALUE,SIZE * SIZE>::const_reference			const_reference;
	typedef typename std::array<VALUE,SIZE * SIZE>::reverse_iterator		reverse_iterator;
	typedef typename std::array<VALUE,SIZE * SIZE>::const_reverse_iterator	const_reverse_iterator;
	typedef typename std::array<VALUE,SIZE * SIZE>::difference_type			difference_type;
	//typedef typename allocator												allocator_type;

private:
	std::array<VALUE, SIZE * SIZE> mTable;
	size_t mDimention = SIZE;
public:
	// default value constructor
	Grid()
	{
	}

	Grid(VALUE v)
	{
		fill(v);
	}


	// copy constructor
	Grid(const Grid& grid)
		:
		mTable(grid.mTable)
	{

	}

	// copy from array
	Grid(const std::array<VALUE, SIZE* SIZE>& grid)
		:
		mTable(grid)
	{

	}

	void operator*=(const int a[SIZE * SIZE])
	{
		std::transform(a, a + SIZE * SIZE, mTable.begin(), std::multiplies);
	}

	bool operator !=(const Grid& b) const
	{
		return mTable != b.mTable && mDimention != b.mDimention;
	}

	//at
	// value from 1d position
	reference at(size_t x)
	{
		return mTable.at(x);
	}

	// value from 1d position
	const_reference at(size_t x) const
	{
		return mTable.at(x);
	}

	// value at coordinates
	reference at(size_t x, size_t y)
	{
		return mTable.at(y * SIZE + x); 
	}

	const_reference at(size_t x, size_t y) const
	{
		return mTable.at(y * SIZE + x);
	}

	reference at(COORD v)
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
	size_t size() const
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


	// iterators

	iterator begin()
	{
		return mTable.begin();
	}

	const_iterator begin() const
	{
		return mTable.begin();
	}

	iterator end()
	{
		return mTable.end();
	}

	const_iterator end() const
	{
		return mTable.end();
	}

	reverse_iterator rbegin()
	{
		return mTable.rbegin();
	}

	reverse_iterator rend()
	{
		return mTable.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return mTable.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return mTable.rend();
	}

	reference front()
	{
		return mTable.front();
	}

	const_reference front() const
	{
		return mTable.front();
	}

	reference back()
	{
		return mTable.back();
	}

	const_reference back() const
	{
		return mTable.back();
	}
};
