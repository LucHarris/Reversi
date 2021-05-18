#include "Vector2i.h"

const std::array< Vector2i, 8> Vector2i::Directions {
		Vector2i{-1,-1}, 		Vector2i{ 0,-1}, 		Vector2i{ 1,-1},	// n/w n n/e
		Vector2i{-1, 0}, 								Vector2i{ 1, 0},	//  w     e		
		Vector2i{-1, 1}, 		Vector2i{ 0, 1}, 		Vector2i{ 1, 1}		// s/w s s/e
};

Vector2i operator+(const Vector2i& l, const Vector2i& r)
{
	return
	{
	l.x + r.x,
	l.y + r.y
	};
}
