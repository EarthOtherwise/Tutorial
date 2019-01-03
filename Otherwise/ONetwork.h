#pragma once
#include"SDL/SDL.h"
#include"NET/SDL_net.h"
#include"MessagingSystem.h"

namespace Otherwise
{

class ONetwork
{
public:
	ONetwork(CorrespondentManager *mcorrManager);
	~ONetwork();

	void initHosting(Uint16 port);
	void quitHosting();

	void update();

	void connectToHost(const char *ip, Uint16 port);
	void recieveConnection();

	void sendData(std::string message, TCPsocket socket);
private:
	bool mConnected = false;
	bool mHost = false;
	TCPsocket mServer;
	TCPsocket mClient;
	SDLNet_SocketSet mSocketSet;
	char dataBuffer[1400];
	Correspondent mCorr;
};

}