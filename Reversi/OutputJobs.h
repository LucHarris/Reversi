#pragma once

class ReversiSFML;
#include "Player.h"
#include "PlayerManager.h"
#include "Board.h"
// contains structs for exchanging data between client and server

struct ClientSendData
{
	// button actions performed by client that are registered by the host
	enum class ButtonOp{NO_OP = -1, 
		SIDE_ADD_WHITE_PL,SIDE_ADD_BLACK_PL, 
		SIDE_ADD_WHITE_AI,SIDE_ADD_BLACK_AI, 
		SIDE_REMOVE_ONE_WHITE,SIDE_REMOVE_ONE_BLACK,
		SIDE_REMOVE_ALL_WHITE,SIDE_REMOVE_ALL_BLACK,
		START_GAME
	};
	enum {MSG_NAME_SIZE = 16, MSG_BODY_SIZE = 48, MSG_SIZE = MSG_NAME_SIZE + MSG_BODY_SIZE	};

	char msg[MSG_SIZE] {'\0'};
	Player player;
	float mouse[2]{ -1.0f,-1.0f };
	int move = -1;
	ButtonOp op = ButtonOp::NO_OP;
	int netPlayerIndex = -1;
	
	// to stop other member variables being evaluated/applied in operator()
	bool dummy = false;

	void operator()(ReversiSFML* d);

	void ToConsole();
	// clients submitted chat messages updates host chat log
	void UpdateHostChat(ReversiSFML* d);
	// clients user data updates host player manager 
	void UpdateHostPlayers(ReversiSFML* d);
	// client board move updates host board
	void UpdateHostMoves(ReversiSFML* d);
	// client button updates host to submit operations
	void UpdateHostButtons(ReversiSFML* d);

	void SetMessage(const std::string& str);
};

// members are copied from the main application
// functor applies changes to the client main application 
struct ServerSendData
{
	enum {	MSG_LENGTH = ClientSendData::MSG_SIZE, MSG_COUNT = 9 }; // match size of chat messages
	// temp data
	//std::array<int,20> moo;
	PlayerManager mPlayerManagerSelect;
	ReversiBoard mBoard;
	std::array<std::array<char, MSG_LENGTH>, MSG_COUNT> mChatLog;
	//char a[500];
	// updates client application
	void operator()(ReversiSFML* pd);
	ServerSendData() = default;

	//  client recv buffer to state
	ServerSendData(const char buffer[], size_t size);

};
