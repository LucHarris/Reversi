#pragma once

#include <array>


struct Vector2i
{
	int x, y;
	Vector2i() : x(0), y(0) {	}
	Vector2i(int a, int b) : x(a), y(b) {	}
	Vector2i(const Vector2i& v) : x(v.x), y(v.y) {	}

	void operator+=(const Vector2i& v)
	{
		x += v.x;
		y += v.y;
	}

	void operator-=(const Vector2i& v)
	{
		x -= v.x;
		y -= v.y;
	}

	Vector2i operator+(const Vector2i& v)
	{
		return { x + v.x, y + v.y };
	}

	static const std::array< Vector2i, 8> Directions;
};

Vector2i operator+(const Vector2i& l, const Vector2i& r)
{
	return 
	{ 
		l.x + r.x, 
		l.y + r.y 
	};
}




