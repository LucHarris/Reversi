#pragma once

struct UserData
{
	char name[16] = "Unnamed";
	unsigned int id = 0;
	int whiteWin = 0;
	int blackWin = 0;
	int draw = 0;
	int gamesPlayed = 0;
	int gamesAbandoned = 0;
};