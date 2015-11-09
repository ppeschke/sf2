#include "NetworkServer.h"
#include <iostream>
using namespace std;

NetworkServer::NetworkServer()
{
	running = true;
	port = 17000;
	AddressLength = sizeof(IncomingAddress);
	WSAStartup(MAKEWORD(2, 2), &Winsock);
	if(LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)
	{
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	for(int i = 0; i < CLIENTS; ++i)
		ZeroMemory(&ClientAddresses[i], sizeof(sockaddr_in));

	ZeroMemory(&ServerAddress, AddressLength);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = port;
	bind(Socket, (sockaddr*)&ServerAddress, AddressLength);

	//UNCOMMENT HERE WHEN READY TO TEST SERVER
	//ListenThreadHandle = CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);
}


NetworkServer::~NetworkServer()
{
	running = false;
	WaitForSingleObject(ListenThreadHandle, INFINITE);	//wait for listen thread to quit
	WSACleanup();
}

void NetworkServer::Stop()
{
	running = false;
}

void NetworkServer::Listen()
{
	while(running)
	{
		if(recvfrom(Socket, Buffer, 256, 0, (sockaddr*)&IncomingAddress, &AddressLength))
		{
			Buffer[255] = '\0';	//security! always end packet with this
			if(Buffer[0] == 1)	//knock packet
			{
				for(int i = 0; i < CLIENTS; ++i)	//search for an empty address
				{
					if(!ClientAddresses[i].sin_family)
					{
						ClientAddresses[i] = IncomingAddress;
						Broadcast("Client has connected. Welcome!");
						break;
					}
				}
			}
			else if(Buffer[0] == 0)	//leave packet
			{
				bool found = false;
				for(int i = 0; i < CLIENTS; ++i)
				{
					if(ClientAddresses[i].sin_addr.s_addr == IncomingAddress.sin_addr.s_addr)
					{
						Buffer[0] = 0;
						Send(Buffer, i);
						ZeroMemory(&ClientAddresses[i], sizeof(sockaddr_in));
						Broadcast("Client has disconnected.");
					}
					else if(ClientAddresses[i].sin_family)
						found = true;
				}
				if(!found)
				{
					running = false;
				}
			}
			else
			{
				Broadcast(Buffer);
			}
		}
	}
}

void NetworkServer::Broadcast(const char* message)
{
	for(int i = 0; i < CLIENTS; ++i)
	{
		if(ClientAddresses[i].sin_family)
			Send(message, i);
	}
}

void NetworkServer::Send(const char* message, unsigned int client)
{
	sendto(Socket, message, 256, 0, (sockaddr*)&ClientAddresses[client], sizeof(sockaddr));
}

DWORD WINAPI ThreadFunction(LPVOID Whatever)
{
	((NetworkServer*)Whatever)->Listen();
	return 0;
}