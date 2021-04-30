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
			std::cout << "\nPop to send";
			j.ToConsole();
			// validate size
			//assert(sizeof(j) == (size_t)sSize && "Client send sizes doesnt match");
			// convert to buffer
			CopyMemory(sBuffer, &j, sSize);
			//CopyMemory(sData, &j, sizeof(sData));
			// send
			std::cout << "\nSend";
			result = send(mSocket, sBuffer, sSize, 0);
			std::cout << "\nSent";


			if (result > 0)
			{

				std::cout << "\nRecv";
				result = recv(mSocket, rBuffer, rSize, 0);
				std::cout << "\nRecv'ed";

				assert(rSize == sizeof(ServerSendData) && "Client recv sizes doesnt match");

				ServerSendData r(rBuffer, rSize);

				mThreadPool->UpdateServerData(r);
				//mThreadPool->PushOutputQueue(r);

				if (result == SOCKET_ERROR)
				{
					std::cout << "Client: socket error " << WSAGetLastError() << "\n";
				}
				else
				{
					std::cout << "Recieved: " << rBuffer << "\t" << result << "\n";
				}
			}
			else if (result == 0)
			{
				std::cout << "Client: Connection to clent closing " << result << "\n";
			}
			else
			{
				std::cout << "Client: send failed " << WSAGetLastError() << "\t" << result << "\n";
			}
		}

	} while (result > 0 && mThreadPool->socketType == ThreadPool::Type::CLIENT_SOCKET);
	
	std::cout << "\nClient body end";
	
}

//todo remove
int ClientSocket::SendRecv(char sData[], int sSize, char rData[], int rSize)
{
	return -1;
}
