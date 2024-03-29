#pragma once
#include "ClientServer.h"

class ServerListener : public ClientServer
{
public:
	ServerListener(ThreadPool* const tp)
		:
		ClientServer(tp)
	{
	}

	~ServerListener();
	// Initialise / listen / loop
	virtual void operator()() override;
	// listener
	virtual bool Body() override;
	// loops listen/accept until broken

};