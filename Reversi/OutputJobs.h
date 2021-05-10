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
	// to stop other member variables being evaluated/applied in operator()
	bool dummy = false;

	void operator()(ReversiSFML* d);

	void ToConsole();
};

// members are copied from the main application
// functor applies changes to the client main application 
struct ServerSendData
{
	// temp data
	//std::array<int,20> moo;
	PlayerManager mPlayerManagerSelect;
	ReversiBoard mBoard;
	//char a[500];
	// updates client application
	void operator()(ReversiSFML* pd);
	ServerSendData() = default;

	//  client recv buffer to state
	ServerSendData(const char buffer[], size_t size);

};
