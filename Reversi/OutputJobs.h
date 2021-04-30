#pragma once

class ReversiSFML;

#include "PlayerManager.h"
#include "Board.h"
// contains structs for exchanging data between client and server

struct ClientSendData
{
	unsigned char msg[32] {0};
	float mouse[2]{ -1.0f,-1.0f };
	int player = -1;
	int move = -1;

	void operator()(ReversiSFML* d);

	void ToConsole();
};

// members are copied from the main application
// functor applies changes to the client main application 
struct ServerSendData
{
	//PlayerManager mPlayerManagerSelect;
	std::array<int,20> moo;
	//ReversiBoard mBoard;
	//char a[500];
	// updates client application
	void operator()(ReversiSFML* pd);
	ServerSendData() = default;

	// copy client recv buffer
	ServerSendData(const char buffer[], size_t size);

};
