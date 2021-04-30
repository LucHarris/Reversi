#include <SFML/Graphics.hpp>
#include "Server.h"
#include <iostream>
#include <assert.h>
#include <WS2tcpip.h>
//#include "ThreadPool.h"
#include "OutputJobs.h"
#include "ReversiSFML.h"

ServerSocket::ServerSocket(const SOCKET& s, ThreadPool* ptp)
	:
	mSocket(s),
	mResult(0),
	mpThreadPool(ptp)
{
}

ServerSocket::~ServerSocket()
{
	std::cout << "\nServer: ~ServerClient() ";
}

// 
int ServerSocket::RecvSend(char* rData, int rSize, char* sData, int sSize)
{
	return -1;
}

void ServerSocket::Body()
{
	const int rSize = sizeof(ClientSendData);
	const int sSize = sizeof(ReversiSFML);
	char rBuffer[rSize] = "[?]";
	char sBuffer[sSize] = "[send from server]";

	int result = 1;
	do
	{
		std::cout << "\nrecv";
		result = recv(mSocket, rBuffer, rSize, 0);
		std::cout << "\nrecv'd";

		if (result == rSize)
		{
			ClientSendData data;
			CopyMemory(&data, rBuffer, sizeof(ClientSendData));

			data.ToConsole();

			mpThreadPool->PushOutputQueue(data);

			Sleep(50); // allow to be processed

			// retrieve data to be sent
			ServerSendData sendData = mpThreadPool->GetServerData();
			CopyMemory(sBuffer, &sendData, sSize);

			std::cout << "\nsend";
			result = send(mSocket, sBuffer, sSize, 0);
			std::cout << "\nsent";

			if (result == SOCKET_ERROR)
			{
				std::cout << "\nServer: socket error  " << WSAGetLastError() << "\n";
			}
			else
			{
				std::cout << "Send: " << sBuffer << "\t " << result << "\n"; // same line as recieved
			}

		}
		Sleep(100);

	} while (result > 0 && mpThreadPool->socketType >= ThreadPool::Type::SERVER_LISTEN);

	std::cout << "do while ended";
}

void ServerSocket::operator()()
{
	std::cout << "\n ServerClient::operator()() Thread process begin \n";

	Body();

}
