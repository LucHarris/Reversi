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
	// assign socket
	mThreadPool->mListener = mSocket;

	Body(); // listen loop


	//shutdown(mSocket, SD_SEND);
	closesocket(mSocket);
	mClosed = true;
	//Close();
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

	// 
	{
		char startMsg[ClientSendData::MSG_SIZE] = "Session Start\0";
		ClientSendData csd;

		std::copy(startMsg, startMsg + ClientSendData::MSG_SIZE, csd.msg);

		mThreadPool->PushOutputQueue(csd);

	}
	
	while(mThreadPool->socketType == ThreadPool::Type::SERVER_LISTEN && mSocketCount < MAX_COUNT)
	{

		result = listen(mSocket, 2);

		std::cout << "Server: listen() for a conection " << result << "\n";

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

			ClientSendData csd;
			csd.SetMessage("Host Listener Closed");
			mThreadPool->PushOutputQueue(csd);
			//assert(false);
		}
		else
		{
			std::cout << "Server:  success" << clientSocket << "\nSocket: " << clientSocket;

			{
				// assign a client to a thread
				ServerSocket sc(clientSocket, mThreadPool);
				mThreadPool->PushThreadQueue(sc,ThreadPool::Type::SERVER_LISTEN);
				++mSocketCount;
			}


		}
	}

	int z = 0;
}

void ServerListener::Body()
{
	// moved to ServerSocket
	Listen();
}
