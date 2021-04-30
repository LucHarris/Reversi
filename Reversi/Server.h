#pragma once
#include "ClientServer.h"

// server side client functor to be passed to a seperate thread
class ServerSocket
{
	SOCKET mSocket;
	HRESULT mResult;
	ThreadPool* mpThreadPool;
public:
	ServerSocket(const SOCKET& s, ThreadPool* ptp);
	~ServerSocket();
	int RecvSend(char* rData, int rSize, char* sData, int sSize);

	void Body();

	void operator()();
};

