#pragma once
#include <WinSock2.h>
//todo add lib: Ws2_32.lib

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN



class ThreadPool;

class ClientServer
{
protected:
	WSAData mWsaData;
	// bind listen for server, connect for client
	SOCKET mSocket;
	//int mResult;
	bool mClosed = false;
	char mPort[6];
	char mAddress[16];
	addrinfo* mInfo = nullptr;
	ThreadPool* const mThreadPool;

private:

public:

	ClientServer(ThreadPool* const tp)
		:
		mPort("8888"),
		mAddress("localhost"),
		mThreadPool(tp)
	{
	}

	~ClientServer();
	
	virtual void operator()() = 0;

	bool Init();

	void Close();

	void SetAddressAndPort();

	virtual bool Body() = 0;

};

