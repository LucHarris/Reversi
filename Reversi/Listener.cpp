#include <iostream>
#include <assert.h>
#include <WS2tcpip.h>
#include "Listener.h"
#include "Server.h"
#include "ThreadPool.h"
//#include "OutputJobs.h"

ServerListener::~ServerListener()
{
	//std::cout << "\n~ServerListener()";
}

void ServerListener::operator()()
{
	Init();
	Body(); // listen loop
}

void ServerListener::Listen()
{

	// setup tcp listening socket
	int result = bind(mSocket, mInfo->ai_addr, (int)mInfo->ai_addrlen);
	std::cout << "Server: bind() : " << result << "\n";

	if (result == SOCKET_ERROR)
	{
		std::cout << "Server: Failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(mInfo);
		closesocket(mSocket);
		assert(false);
	}
	else
	{
		std::cout << "Server: Succeeded (info no longer needed) " << result << " \n";
		freeaddrinfo(mInfo);
	}

	
	while(mThreadPool->socketType == ThreadPool::Type::SERVER_LISTEN)
	{

		result = listen(mSocket, 5);

		std::cout << "Server: listen() for a single conection " << result << "\n";

		if (result == SOCKET_ERROR)
		{
			std::cout << "Server:  fail " << WSAGetLastError() << "\n";
			closesocket(mSocket);
			WSACleanup();
			assert(false);
		}
		else
		{
			std::cout << "Server: listen success " << result << "\n";
		}

		//Accept a client socket
		std::cout << "Server: accept() start...\n";
		SOCKET clientSocket = accept(mSocket, nullptr, nullptr);

		std::cout << "Server: accept() end...\n ";

		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Server: invalid socket " << WSAGetLastError() << "\n";
			closesocket(clientSocket);
			WSACleanup();
			assert(false);
		}
		else
		{
			std::cout << "Server:  success" << clientSocket << "\nSocket: " << clientSocket;

			{
				// assign a client to a thread
				ServerSocket sc(clientSocket, mThreadPool);
				mThreadPool->PushThreadQueue(sc,ThreadPool::Type::SERVER_LISTEN);
			}


		}

	}
}

void ServerListener::Body()
{
	// moved to ServerSocket
	Listen();
}