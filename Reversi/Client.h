#pragma once
#include <SFML/Graphics.hpp>
#include "ClientServer.h"

class ClientSocket : public ClientServer
{
public:
	ClientSocket(ThreadPool* const tp)
		:
		ClientServer(tp)
	{

	}

	// init / connect / loop
	virtual void operator()() override;

	bool Connect();

	// for send/recv
	virtual bool Body() override;

	int SendRecv(char sData[], int sSize, char rData[], int rSize);
};
