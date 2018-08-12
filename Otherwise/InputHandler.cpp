#include "InputHandler.h"

namespace Otherwise
{

	InputHandler::InputHandler()
	{
	}


	InputHandler::~InputHandler()
	{
	}

	void Otherwise::InputHandler::pressKey(unsigned int keyID)
	{
		mKeyMap[keyID] = true;
	}

	void Otherwise::InputHandler::unPressKey(unsigned int keyID)
	{
		mKeyMap[keyID] = false;
	}

	bool Otherwise::InputHandler::isKeyDown(unsigned int keyID)
	{
		auto it = mKeyMap.find(keyID);
		if (it != mKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	void InputHandler::inputQueue()
	{
		SDL_Event evnt;

		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
			case SDL_QUIT:
				SDL_Quit();
				exit(60);
				break;
			case SDL_KEYDOWN:
				pressKey(evnt.key.keysym.sym);
				break;
			default:
				break;
			}
		}
	}

}