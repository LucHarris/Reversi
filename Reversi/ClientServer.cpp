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

void ClientServer::Init()
{
	

	const addrinfo hints = {
		AI_PASSIVE,
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP
	};

	int result = WSAStartup(MAKEWORD(2, 2), &mWsaData);

	if (result != 0)
	{
		std::cout << "Init: WSAStartup() failed:" << result << "\n";
		assert(false);
	}
	else
	{
		std::cout << "Init: WSAStartup() success:" << result << "\n";

	}

	result = getaddrinfo(mAddress, "8888", &hints, &mInfo);

	if (result != 0)
	{
		std::cout << "Init: getaddrinfo() failed:" << result << "\n";

		Close();
		assert(false);
	}
	else
	{
		std::cout << "Init: getaddrinfo() success: " << result << "\n";
	}

	// setup socket
	mSocket = socket(mInfo->ai_family, mInfo->ai_socktype, mInfo->ai_protocol);
	if (mSocket == INVALID_SOCKET)
	{
		std::cout << "Init: Unable to create server socket:" << result << "\n";
		freeaddrinfo(mInfo);
		WSACleanup();
		assert(false);
	}
	else
	{
		std::cout << "Init: success creating server socket:" << result << "\n";

	}

	// connect // listen comes next

}

//void ClientServer::Send(char buffer[], size_t size)
//{
//	result = send(mSocket, buffer, size, 0);
//}

//void ClientServer::Recv(char buffer[], size_t size)
//{
//	mResult = recv(mSocket, buffer, size, 0);
//}

//int ClientServer::GetResult() const
//{
//	return mResult;
//}

void ClientServer::Close()
{
	//freeaddrinfo(mInfo); // maybe only in setting up
	closesocket(mSocket);
	WSACleanup(); // maybe not during certain setup
	mClosed = true;
}

void ClientServer::SetAddressAndPort()
{
	//if (sizeof(*add) / sizeof(add[0]) <= sizeof(*mAddress) / sizeof(mAddress[0]) &&
	//	sizeof(*port) / sizeof(port[0]) <= sizeof(*mPort) / sizeof(mPort[0])
	//	)
	//{
	//	//todo simlify
	//	*mAddress = *add;
	//	*mPort = *port;
	//}
	//else
	//{
	//	assert(false);
	//}
}
