#include "ONetwork.h"
#include"ErrHandler.h"
#include<cstring>
#include<string>
#include<iostream>

namespace Otherwise
{

ONetwork::ONetwork(CorrespondentManager *mcorrManager)
{
	SDLNet_Init();
	mSocketSet = SDLNet_AllocSocketSet(10);

	mCorr.init(mcorrManager, std::string("Network"));
}

ONetwork::~ONetwork()
{
	SDLNet_FreeSocketSet(mSocketSet);
	SDLNet_Quit();
}

void Otherwise::ONetwork::initHosting(Uint16 port)
{
	IPaddress serverIP;
	SDLNet_ResolveHost(&serverIP, NULL, port);
	mServer = SDLNet_TCP_Open(&serverIP);
	mHost = true;
}

void Otherwise::ONetwork::quitHosting()
{
	SDLNet_TCP_Close(mClient);
	SDLNet_TCP_Close(mServer);
	mHost = false;
}

void Otherwise::ONetwork::update()
{
	if (mHost && !mConnected)
	{
		recieveConnection();
	}

	if (mCorr.getMessage())
	{
		sendData(mCorr.getStringMessage(), mClient);
		mCorr.clearMessage();
	}

	while (SDLNet_CheckSockets(mSocketSet, 0) > 0)
	{
		if (SDLNet_SocketReady(mClient))
		{
			SDLNet_TCP_Recv(mClient, dataBuffer, 1400);
			mCorr.publish(std::string(dataBuffer));
		}
	}
}

void Otherwise::ONetwork::connectToHost(const char * ip, Uint16 port)
{
	IPaddress IP;
	if (SDLNet_ResolveHost(&IP, ip, port) == -1)
	{
		throwError("Connection", "Couldn't connect to server");
	}

	mClient = SDLNet_TCP_Open(&IP);

	if (mClient == NULL)
	{
		throwError("Connection", "Couldn't connect to server, bad ip");
	}

	SDLNet_TCP_AddSocket(mSocketSet, mClient);
}

void Otherwise::ONetwork::recieveConnection()
{
	mClient = SDLNet_TCP_Accept(mServer);
	if (mClient)
	{
		SDLNet_TCP_AddSocket(mSocketSet, mClient);
		mConnected = true;
	}
}

void Otherwise::ONetwork::sendData(std::string message, TCPsocket socket)
{
	sprintf_s(dataBuffer, "%s", message.c_str());
	SDLNet_TCP_Send(socket, dataBuffer, 1400);
}

}