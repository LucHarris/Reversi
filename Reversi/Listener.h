#pragma once
#include "ClientServer.h"

class ServerListener : public ClientServer
{
	int mSocketCount = 0;
	const int MAX_COUNT = 2;
public:
	ServerListener(ThreadPool* const tp)
		:
		ClientServer(tp)
	{

	}

	~ServerListener();

	// Initialise / listen / loop
	virtual void operator()() override;
	
	virtual void Body() override;
	// loops listen/accept until broken
	void Listen();

};