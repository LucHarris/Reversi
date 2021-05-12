#pragma once

struct UserData
{
	char name[16] = "Unnamed";
	unsigned int id;
	int whiteWin = 0;
	int blackWin = 0;
	int draw = 0;
	int gamesPlayed = 0;
	int gamesAbandoned = 0;

	UserData()
		:
		id(rand() % INT_MAX)
	{

	}

	static size_t GetMaxNameSize()
	{
		return sizeof(name) / sizeof(name[0]);
	}
};