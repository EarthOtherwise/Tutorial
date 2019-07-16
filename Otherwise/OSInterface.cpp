#include "OSInterface.h"
#include "ErrHandler.h"
#include <SDL/SDL_timer.h>
#include <GL/glew.h>

namespace Otherwise
{
	unsigned millisecondsSinceProgramStart()
	{
		return static_cast<unsigned int>(SDL_GetTicks());
	}

	int relativeMouseMotionToggle()
	{
		if (SDL_GetRelativeMouseMode() == SDL_TRUE)
		{
			return SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		return SDL_SetRelativeMouseMode(SDL_TRUE);
	}

OSInterface::OSInterface()
{
}


OSInterface::~OSInterface()
{
	SDL_Quit();
}

	void OSInterface::init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	int OSInterface::createWindow(std::string windowName, unsigned int screenWidth, unsigned int screenHeight, int currentFlags)
{
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE)
	{
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}

	mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

	SDL_GLContext glContext = SDL_GL_CreateContext(mSDLWindow);

	glewInit();

	glClearDepth(1.0);

	glClearColor(0.5f, 0.0f, 0.5f, 0.5f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);


	return 0;
}

	void OSInterface::updateInput()
	{
		SDL_Event evnt;
		while(SDL_PollEvent(&evnt))
		{
			mEvents.push_back(evnt);
			MouseState mouseState;
			KeyState keyState;
			MouseMotion mouseMotion;

			switch (evnt.type)
			{
			case SDL_KEYDOWN:
				if (evnt.key.windowID == SDL_GetWindowID(mSDLWindow))
				{
					keyState.key = evnt.key.keysym.sym;
					keyState.down = true;
					mInputState.mKeys.push_back(keyState);
				}
					break;
				case SDL_KEYUP:
					if (evnt.key.windowID == SDL_GetWindowID(mSDLWindow))
					{
						keyState.key = evnt.key.keysym.sym;
						keyState.down = false;
						mInputState.mKeys.push_back(keyState);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (evnt.button.windowID == SDL_GetWindowID(mSDLWindow))
					{
						mouseState.button = evnt.button.button;
						mouseState.down = true;
						mouseState.x = evnt.button.x;
						mouseState.y = evnt.button.y;
						mInputState.mButtons.push_back(mouseState);
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (evnt.button.windowID == SDL_GetWindowID(mSDLWindow))
					{
						mouseState.button = evnt.button.button;
						mouseState.down = false;
						mouseState.x = evnt.button.x;
						mouseState.y = evnt.button.y;
						mInputState.mButtons.push_back(mouseState);
					}
					break;
				case	SDL_MOUSEMOTION:
					if (evnt.motion.windowID == SDL_GetWindowID(mSDLWindow))
					{
						mouseMotion.x = evnt.motion.x;
						mouseMotion.y = evnt.motion.y;
						mouseMotion.xRel = evnt.motion.xrel;
						mouseMotion.yRel = evnt.motion.yrel;
						mInputState.mMotions.push_back(mouseMotion);
					}
					break;
				default:
					break;
			}
		}
	}

	void Otherwise::OSInterface::swapBuffer()
{
	SDL_GL_SwapWindow(mSDLWindow);
}

std::vector<SDL_Event> OSInterface::getEvents()
{
	updateInput();
	std::vector<SDL_Event> returnVector;
	returnVector = mEvents;
	mEvents.clear();
	return returnVector;
}

InputState OSInterface::getInput()
{
	updateInput();
	InputState returnState;
	returnState = mInputState;
	mInputState.mButtons.clear();
	mInputState.mKeys.clear();
	mInputState.mMotions.clear();
	return returnState;
}

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