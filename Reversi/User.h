#pragma once

struct UserData
{
	enum { NAME_SIZE = 16 };
	char name[16] = "-\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	unsigned int id;
	int whiteWin = 0;
	int blackWin = 0;
	int draw = 0;
	int gamesPlayed = 0;
	int totalWins = 0;

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