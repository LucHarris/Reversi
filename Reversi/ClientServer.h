#pragma once
#include <WinSock2.h>
//todo add lib: Ws2_32.lib

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN



class ThreadPool;

class ClientServer
{
public:
	enum{PORT_SIZE = 6, ADDRESS_SIZE = 16};
protected:
	WSAData mWsaData;
	// bind listen for server, connect for client
	SOCKET mSocket;
	//int mResult;
	bool mClosed = false;
	char mPort[PORT_SIZE];
	char mAddress[ADDRESS_SIZE];
	addrinfo* mInfo = nullptr;
	ThreadPool* const mThreadPool;

private:

public:

	ClientServer(ThreadPool* const tp)
		:
		mPort("8888"),			// overriden in nextowk state
		mAddress("localhost"),	// overriden in nextowk state
		mThreadPool(tp)
	{
	}

	~ClientServer();
	
	virtual void operator()() = 0;
	bool Init();
	void Close();
	void SetAddressAndPort(const char p[PORT_SIZE],const char a[ADDRESS_SIZE]);
	virtual bool Body() = 0;

};

