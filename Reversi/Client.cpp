#include "Client.h"
#include <iostream>
#include <WS2tcpip.h>
#include <cassert>
#include "ThreadPool.h"
#include "OutputJobs.h"
#include <limits>




void ClientSocket::operator()()
{
	Init();
	Connect();
	Body();
	Close();
}

void ClientSocket::Connect()
{
	int result = connect(mSocket, mInfo->ai_addr, (int)mInfo->ai_addrlen);

	if ( result == SOCKET_ERROR)
	{
		std::cout << "Client: Unable to connect" << result << "\n";
		freeaddrinfo(mInfo);
		closesocket(mSocket);
		WSACleanup();
		assert(false);
	}
}

void ClientSocket::Body()
{
	
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

				//assert(rSize == sizeof(ServerSendData) && "Client recv sizes doesnt match");

				ServerSendData r(rBuffer, rSize);

				mThreadPool->UpdateServerData(r);
				//mThreadPool->PushOutputQueue(r);

				if (result == SOCKET_ERROR)
				{
					std::cout << "\tClient: socket error " << WSAGetLastError() ;
				}
				else
				{
					std::cout << "\tRecieved data. Size " << rBuffer << "\t" << result;
				}
			}
			else if (result == 0)
			{
				std::cout << "\tClient: Connection to clent closing " << result;
			}
			else
			{
				std::cout << "\tClient: send failed " << WSAGetLastError() << "\t" << result;
			}
		}
		else
		{
			//std::cout << "\nnothing to pop from input queue";
		}

	} while (result > 0 && mThreadPool->socketType == ThreadPool::Type::CLIENT_SOCKET);
	
	std::cout << "\nClient body end";

}

//todo remove
int ClientSocket::SendRecv(char sData[], int sSize, char rData[], int rSize)
{
	return -1;
}
