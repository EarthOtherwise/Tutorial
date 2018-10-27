#pragma once
#include<unordered_map>
#include "ErrHandler.h"
#include<SDL/SDL.h>
#include "glm/glm.hpp"
#include "MessagingSystem.h"

namespace Otherwise
{
	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		void init(CorrespondentManager *corrManager);

		/*Called when the player presses a key so that it can be checked against the keybindings*/
		void pressKey(unsigned int keyID);

		void pressButton(unsigned int keyID, glm::vec2 mousePosition);

		void findAllKeys(std::string &filePath);

		void mapKey(unsigned int keyID, std::string &signature);

		/*Logs keystrokes to be used by the game.*/
		void inputQueue();

	private:
		std::unordered_map<unsigned int, Correspondent> mKeyMap;
		CorrespondentManager *mCorrespondentManager;
		Correspondent mToGUI;
		Correspondent mMouseSender;
	};

}