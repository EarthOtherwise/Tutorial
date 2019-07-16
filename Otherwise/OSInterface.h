#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include <vector>
#include <string>
#include <NET/SDL_net.h>
#include "MessagingSystem.h"

namespace Otherwise
{

unsigned int millisecondsSinceProgramStart();

int relativeMouseMotionToggle();

	struct KeyState
	{
		unsigned int key = 0;
		bool down = false;
	};

	struct KeyCondition
	{
		bool keyConditions(KeyState * state) const
		{
			return state->key == key && state->down == down;
		}
		unsigned int key = 0;
		bool down = false;
	};

	struct MouseState
	{
		unsigned int button = 0;
		bool down = false;
		int x = 0;
		int y = 0;
	};

	struct MouseCondition
	{
		bool mouseConditions(MouseState * state) const
		{
			return state->button == button && state->down == down;
		}
		unsigned int button = 0;
		bool down = false;
	};

	struct MouseMotion
	{
		int x = 0;
		int y = 0;
		int xRel = 0;
		int yRel = 0;
	};

	struct InputState
	{
		std::vector<KeyState> mKeys;
		std::vector<MouseState> mButtons;
		std::vector<MouseMotion> mMotions;
	};

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

class OSInterface
{
public:
	OSInterface();
	~OSInterface();

	void init();

	int createWindow(std::string windowName, unsigned int screenWidth, unsigned int screenHeight, int currentFlags);

	void updateInput();

	void swapBuffer();

	std::vector<SDL_Event> getEvents();
	InputState getInput();

private:
	InputState mInputState;
	std::vector<SDL_Event> mEvents;
	SDL_Window* mSDLWindow;
};

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