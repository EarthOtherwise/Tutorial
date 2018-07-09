#pragma once
#include<unordered_map>
#include "ErrHandler.h"
#include<SDL/SDL.h>


namespace Otherwise
{
	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		/*Called when the player presses a key so that it can be checked against the keybindings*/
		void pressKey(unsigned int keyID);

		/*Check if a key is currently down.*/
		bool isKeyDown(unsigned int keyID);

		/*Logs keystrokes to be used by the game.*/
		void inputQueue();

	private:
		std::unordered_map<unsigned int, bool> mKeyMap;
	};

}