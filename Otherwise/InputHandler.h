#pragma once
#include<unordered_map>
#include "ErrHandler.h"
#include<SDL/SDL.h>
#include "glm/glm.hpp"
#include "GUI.h"

namespace Otherwise
{
	struct OtherwiseInput
	{
		bool keyDown = false;
	};

	struct OKey : public OtherwiseInput
	{
		void(*keyFunction)();
	};

	struct OButton
	{
		void(*buttonFunction)(glm::vec2);
	};

	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		void init(GUI* gui);

		/*Called when the player presses a key so that it can be checked against the keybindings*/
		void pressKey(unsigned int keyID);
		void unpressKey(unsigned int keyID);

		void pressButton(unsigned int keyID, glm::vec2 mousePosition);
		void unpressButton(unsigned int keyID, glm::vec2 mousePosition);

		void mapKeysFromFile(std::string &filePath, 
			std::unordered_map<std::string, unsigned int> initStringsToSDLKeycodes, 
			std::unordered_map<std::string, void(*)()> initStringsToFunctions,
			std::unordered_map<std::string, void(*)(glm::vec2)> buttonmapStringsToFunctions);

		/*Logs keystrokes to be used by the game.*/
		void inputQueue();

	private:
		std::unordered_map<unsigned int, OKey> mKeyMap;
		std::unordered_map<unsigned int, OButton> mButtonMap;
		GUI* mGUI = nullptr;
	};

}