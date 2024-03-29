#include <SFML/Graphics.hpp>
#include "Server.h"
#include <iostream>
#include <assert.h>
#include <WS2tcpip.h>
#include "OutputJobs.h"
#include "ReversiSFML.h"
#include <string.h>


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

void ServerSocket::Body()
{
	ClientSendData csd;

	const int rSize = sizeof(ClientSendData);
	const int sSize = sizeof(ServerSendData);
	char rBuffer[rSize];
	char sBuffer[sSize];

	int result = 1;

	mpThreadPool->PushSocketTracker(mSocket);

	do
	{
		std::cout << "\nrecv";
		result = recv(mSocket, rBuffer, rSize, 0);
		std::cout << "\t recv'd";

		if (result > 0)
		{
			ClientSendData data;
			CopyMemory(&data, rBuffer, sizeof(ClientSendData));
			// to compare against thread pool active socket list
			data.player.serverSocket = mSocket;
			data.ToConsole();

			mpThreadPool->PushOutputQueue(data);

			// allow to be processed
			Sleep(50); 

			// retrieve data to be sent
			ServerSendData sendData = mpThreadPool->GetServerData();
			CopyMemory(sBuffer, &sendData, sSize);

			std::cout << "\t send";
			result = send(mSocket, sBuffer, sSize, 0);
			std::cout << "\t sent";

			if (result == SOCKET_ERROR)
			{
				csd.SetMessage("Socket error: " + std::to_string(WSAGetLastError()) + " Socket: " + std::to_string(mSocket));
				mpThreadPool->PushOutputQueue(csd);

				std::cout << "\n\n Server: socket error  " << WSAGetLastError() << "\n";
			}
			else
			{
				// success
				std::cout << "\t Send data. Size " << result; // same line as recieved
			}
		}
		else
		{
			std::cout << "\t result == rSize = false result: " << result << "rSize: "<< rSize << " error: " << WSAGetLastError() << "  ";
		}

		Sleep(100);

	} while (result > 0 && mpThreadPool->socketType >= ThreadPool::Type::SERVER_LISTEN);

	mpThreadPool->PopSockerTracker(mSocket);


	std::cout << "\ndo while ended";
}

void ServerSocket::operator()()
{
	std::cout << "\n ServerClient::operator()() Thread process begin \n";

	++mpThreadPool->socketCount;
	Body();
	shutdown(mSocket, SD_SEND);
	closesocket(mSocket);
	WSACleanup();
	--mpThreadPool->socketCount;
}
