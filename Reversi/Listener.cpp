#include <iostream>
#include <assert.h>
#include <WS2tcpip.h>
#include "Listener.h"
#include "Server.h"
#include "ThreadPool.h"
//#include "OutputJobs.h"

ServerListener::~ServerListener()
{
}

void ServerListener::operator()()
{
	if (Init())
	{
		// assign socket
		mThreadPool->mListener = mSocket;

		Body();
	}
	
	mClosed = true;
	shutdown(mSocket, SD_SEND);
	Close();
}

bool ServerListener::Body()
{
	ClientSendData csd;

	// setup tcp listening socket
	int result = bind(mSocket, mInfo->ai_addr, (int)mInfo->ai_addrlen);
	std::cout << "Server: bind() : " << result << "\n";

	if (result == SOCKET_ERROR)
	{
		std::cout << "Server: Failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(mInfo);
		closesocket(mSocket);

		csd.SetMessage("Unable to bind: " + std::to_string(WSAGetLastError()));
		mThreadPool->PushOutputQueue(csd);
		//assert(false);
		return false;
	}
	else
	{
		std::cout << "Server: Succeeded (info no longer needed) " << result << " \n";
		freeaddrinfo(mInfo);
	}

	// starting session message
	{
		char startMsg[ClientSendData::MSG_SIZE] = "Session Start\0";
		ClientSendData csd;

		std::copy(startMsg, startMsg + ClientSendData::MSG_SIZE, csd.msg);

		mThreadPool->PushOutputQueue(csd);
	}

	while (mThreadPool->socketType == ThreadPool::Type::SERVER_LISTEN && mThreadPool->socketCount <= ThreadPool::MAX_PLAYERS)
	{

		result = listen(mSocket, 1024);

		std::cout << "Server: listen() for a conection " << result << "\n";

		if (result == SOCKET_ERROR)
		{
			std::cout << "Server:  fail " << WSAGetLastError() << "\n";
			closesocket(mSocket);
			WSACleanup();


			csd.SetMessage("Unable to listen for a connection");
			mThreadPool->PushOutputQueue(csd);

			return false;
			//assert(false);
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

			csd.SetMessage("Host ended");
			mThreadPool->PushOutputQueue(csd);


			return false;
			//assert(false);
		}
		else
		{
			std::cout << "Server:  success" << clientSocket << "\nSocket: " << clientSocket;

			{
				// assign a client to a thread
				ServerSocket sc(clientSocket, mThreadPool);
				mThreadPool->PushThreadQueue(sc, ThreadPool::Type::SERVER_LISTEN);
			}
		}
	}

	return true;
}
