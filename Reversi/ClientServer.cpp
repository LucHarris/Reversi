#include <SFML/Graphics.hpp>
#include "ClientServer.h"
#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cassert>
#include <WinSock2.h>
#include "ThreadPool.h"

ClientServer::~ClientServer()
{
	// tidy up if not done so
	if (!mClosed)
	{
		Close();
	}
}

bool ClientServer::Init()
{
	

	const addrinfo hints = {
		AI_PASSIVE,
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP
	};

	int result = WSAStartup(MAKEWORD(2, 2), &mWsaData);

	ClientSendData csd;
	if (result != 0)
	{
		std::cout << "Init: WSAStartup() failed:" << result << "\n";

		csd.SetMessage("WSAStartup failed: " + std::to_string(result));
		mThreadPool->PushOutputQueue(csd);

		return false;
	}
	else
	{
		std::cout << "Init: WSAStartup() success:" << result << "\n";

	}

	result = getaddrinfo(mAddress, mPort, &hints, &mInfo);

	if (result != 0)
	{
		std::cout << "Init: getaddrinfo() failed:" << result << "\n";

		csd.SetMessage("getaddrinfo failed: " + std::to_string(result));
		mThreadPool->PushOutputQueue(csd);

		Close();

		return false;
	}
	else
	{
		std::cout << "Init: getaddrinfo() success: " << result << "\n";
	}

	// setup socket
	mSocket = socket(mInfo->ai_family, mInfo->ai_socktype, mInfo->ai_protocol);
	if (mSocket == INVALID_SOCKET)
	{
		std::cout << "Init: Unable to create server socket:" << mSocket << "\n";

		csd.SetMessage("Unable to create server socket: " + std::to_string(mSocket));
		mThreadPool->PushOutputQueue(csd);

		freeaddrinfo(mInfo);
		WSACleanup();

		return false;
	}
	else
	{
		std::cout << "Init: success creating server socket:" << mSocket << "\n";
	}

	return true;
}

void ClientServer::Close()
{
	closesocket(mSocket);
	WSACleanup(); 
	mClosed = true;
}

void ClientServer::SetAddressAndPort(const char p[PORT_SIZE],const char a[ADDRESS_SIZE])
{
	std::copy(p, p + PORT_SIZE, mPort);
	std::copy(a, a + ADDRESS_SIZE, mAddress);

}
