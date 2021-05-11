#include "OutputJobs.h"
#include "ReversiSFML.h"
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <cassert>

// called in host to update main game
void ClientSendData::operator()(ReversiSFML* d)
{
	if (!dummy)
	{
		//todo apply client message
		if (msg[0] != '\0')
		{
			d->chat.AddMessage(msg);
		}
		//todo apply client mouse pos 
		//todo apply client player
		//todo apply client move
		if (move >= 0 && move < 64)
		{
			if (d->reversiGame.CanMove())
			{
				d->reversiGame.Move(move);
			}
			//std::cout << "\nclient move to " << move;
		}
		ToConsole();
	}
}

void ClientSendData::ToConsole()
{
	if (!dummy)
	{
	std::cout <<'\t'
		<< dummy << '\t'
		<< msg << '\t'
		<< mouse[0] << '\t'
		<< mouse[1] << '\t'
		<< player << '\t'
		<< move << '\t'
		;
	}
	else
	{
		std::cout << "\t\t\t\t\t\t\t";
	}
}

void ServerSendData::operator()(ReversiSFML* pd)
{
	pd->PlayerSelection = mPlayerManagerSelect;
	pd->reversiGame = mBoard;
	//std::cout << "\nServerSendData::operator()(ReversiSFML* pd)";
}

ServerSendData::ServerSendData(const char buffer[], size_t size)
{
	assert(size == sizeof(*this));

	memcpy_s(this, sizeof(ServerSendData), buffer, size);
}