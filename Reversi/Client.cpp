#include "Client.h"
#include <iostream>
#include <WS2tcpip.h>
#include <cassert>
#include "ThreadPool.h"
#include "OutputJobs.h"
#include <limits>




void ClientSocket::operator()()
{
	if (Init())
	{
		if (Connect())
		{
			Body();
		}
	}
	shutdown(mSocket, SD_SEND);
	Close();
}

bool ClientSocket::Connect()
{
	int result = connect(mSocket, mInfo->ai_addr, (int)mInfo->ai_addrlen);

	ClientSendData csd;

	if ( result == SOCKET_ERROR)
	{
		csd.SetMessage("Failed to connect to host...");
		mThreadPool->PushOutputQueue(csd);

		std::cout << "Client: Unable to connect" << result << "\n";
		freeaddrinfo(mInfo);
		closesocket(mSocket);
		WSACleanup();
		
		return false;
	}
	else
	{
		return true;
	}
}

bool ClientSocket::Body()
{
	ClientSendData csd;
	
	int result = 1;
	//char sBuffer[buffer_size] = "[send from client]";
	const int sSize = (int)sizeof(ClientSendData);
	const int rSize = (int)sizeof(ServerSendData);
	char sBuffer[sSize];
	char rBuffer[rSize];
	std::cout << "\nSocket: " << mSocket << "\n";

	do
	{
		//std::cout << "\nDoWhile start";
		ClientSendData j;
		if (mThreadPool->PopInputQueue(j))
		{
			//std::cout << "\nPop to send from input queue";

			// convert to buffer
			CopyMemory(sBuffer, &j, sSize);
			//CopyMemory(sData, &j, sizeof(sData));
			// send
			std::cout << "\nSend";
			result = send(mSocket, sBuffer, sSize, 0);
			std::cout << "\tSent";

			if (result > 0)
			{

				std::cout << "\tRecv";
				result = recv(mSocket, rBuffer, rSize, 0);
				std::cout << "\tRecv'ed";

				ServerSendData r(rBuffer, rSize);

				mThreadPool->UpdateServerData(r);

				if (result == SOCKET_ERROR)
				{
					std::cout << "\tClient: socket error " << WSAGetLastError() ;

					csd.SetMessage("Socket error: " + std::to_string(WSAGetLastError()));
					mThreadPool->PushOutputQueue(csd);
				}
				else
				{
					// success
					std::cout << "\tRecieved data. Size " << rBuffer << "\t" << result;
				}
			}
			else
			{
				if (result == 0)
				{
					std::cout << "\tClient: Connection to client closing " << result;
					csd.SetMessage("Leaving Session...");
					mThreadPool->PushOutputQueue(csd);
				}
				else
				{
					csd.SetMessage("Unable to send" + std::to_string(WSAGetLastError()));
					mThreadPool->PushOutputQueue(csd);

					std::cout << "\tClient: send failed " << WSAGetLastError() << "\t" << result;
				}
			}
		}
		else
		{
			//std::cout << "\nnothing to pop from input queue";
		}

	} while (result > 0 && mThreadPool->socketType == ThreadPool::Type::CLIENT_SOCKET);
	
	if (mThreadPool->socketType != ThreadPool::Type::END)
	{
		mThreadPool->socketType = ThreadPool::Type::NONE;
	}

	std::cout << "\nClient body end";

	return true;
}

//todo remove
int ClientSocket::SendRecv(char sData[], int sSize, char rData[], int rSize)
{
	return -1;
}
