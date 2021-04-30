#include "OutputJobs.h"
#include "ReversiSFML.h"
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <cassert>


void ClientSendData::operator()(ReversiSFML* d)
{
	std::cout << "\nApply to main data here";
	
	
	ToConsole();

}

void ClientSendData::ToConsole()
{
	std::cout <<'\n'
		<< msg << '\t'
		<< mouse[0] << '\t'
		<< mouse[1] << '\t'
		<< player << '\t'
		<< move << '\t'
		;
}

void ServerSendData::operator()(ReversiSFML* pd)
{
	//pd->PlayerSelection = mPlayerManagerSelect;
	//pd->reversiGame = mBoard;

	std::cout << "\nServerSendData::operator()(ReversiSFML* pd)";
}

ServerSendData::ServerSendData(const char buffer[], size_t size)
{
	assert(size == sizeof(*this));

	memcpy_s(this, sizeof(ServerSendData), buffer, size);
}

