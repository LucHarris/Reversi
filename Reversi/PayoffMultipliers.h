#pragma once
#include <array>
#include <string>

class PayoffMultipliers
{
	std::array<std::array<int, 64>, 3> mPayoffMultipliers;
	const std::string mPaths[3];
public:
	enum { EVEN = 0, PREDEFINED, ADAPTIVE, COUNT };

	PayoffMultipliers();

	void Load();

	const std::array<int, 64>& GetAt(size_t i) const;
	
};

