#include "InputHandler.h"
#include "InitFileReadWrite.h"

namespace Otherwise
{

	InputHandler::InputHandler()
	{
	}


	InputHandler::~InputHandler()
	{
	}

	void InputHandler::init(GUI * gui)
	{
		mGUI = gui;
	}

	void Otherwise::InputHandler::pressKey(unsigned int keyID)
	{
		auto it = mKeyMap.find(keyID);
		if (it != mKeyMap.end())
		{
			it->second.keyFunction();
		}
	}

	void Otherwise::InputHandler::unpressKey(unsigned int keyID)
	{
		auto it = mKeyMap.find(keyID);
		if (it != mKeyMap.end())
		{
			it->second.keyFunction();
		}
	}

	void Otherwise::InputHandler::pressButton(unsigned int keyID, glm::vec2 mousePosition)
	{
		auto it = mButtonMap.find(keyID);
		if (it != mButtonMap.end())
		{
			it->second.buttonFunction(mousePosition);
		}
	}

	void Otherwise::InputHandler::unpressButton(unsigned int keyID, glm::vec2 mousePosition)
	{
		auto it = mButtonMap.find(keyID);
		if (it != mButtonMap.end())
		{
			it->second.buttonFunction(mousePosition);
		}
	}

	void InputHandler::mapKeysFromFile(std::string & filePath,
		std::unordered_map<std::string, unsigned int> initStringsToSDLKeycodes, 
		std::unordered_map<std::string, void(*)()> initStringsToFunctions,
		std::unordered_map<std::string, void(*)(glm::vec2)> buttonmapStringsToFunctions)
	{
		std::vector<std::string> keymapFileStrings = readFile(filePath);

		unsigned int iterator = 0;
		while (keymapFileStrings[iterator] != "KeyMap")
		{
			if (iterator > keymapFileStrings.size())
			{
				throwError("keymapError", "keymap file " + filePath + " contains no labeled 'KeyMap'");
			}
			iterator++;
		}

		iterator++;

		while (keymapFileStrings[iterator] != "KeyMapEnd")
		{
			if (iterator > keymapFileStrings.size())
			{
				throwError("keymapError", "keymap file " + filePath + " contains no labeled 'KeyMapEnd'");
			}

			unsigned int newKey = 0;
			OKey newKeyFunction;

			auto it = initStringsToSDLKeycodes.find(keymapFileStrings[iterator]);
			if (it != initStringsToSDLKeycodes.end())
			{
				newKey =  it->second;
			}
			else
			{
				throwError("keymapSyntax", "Incorrect Keymap Syntax, no key where it was expected, instead I got: " + keymapFileStrings[iterator]);
			}

			iterator++;

			auto knit = initStringsToFunctions.find(keymapFileStrings[iterator]);
			if (knit != initStringsToFunctions.end())
			{
				newKeyFunction.keyFunction = knit->second;
			}
			else
			{
				throwError("keymapSyntax", "Incorrect Keymap Syntax, no function where it was expected, instead I got: " + keymapFileStrings[iterator]);
			}

			mKeyMap[newKey] = newKeyFunction;

			iterator++;
		}
		
		iterator = 0;

		while (keymapFileStrings[iterator] != "ButtonMap")
		{
			if (iterator > keymapFileStrings.size())
			{
				throwError("buttonmapError", "buttonmap file " + filePath + " contains no labeled 'KeyMap'");
			}
			iterator++;
		}

		iterator++;

		while (keymapFileStrings[iterator] != "ButtonMapEnd")
		{
			if (iterator > keymapFileStrings.size())
			{
				throwError("buttonmapError", "buttonmap file " + filePath + " contains no labeled 'KeyMapEnd'");
			}

			unsigned int newButton = 0;
			OButton newButtonFunction;

			auto it = initStringsToSDLKeycodes.find(keymapFileStrings[iterator]);
			if (it != initStringsToSDLKeycodes.end())
			{
				newButton = it->second;
			}
			else
			{
				throwError("buttonmapSyntax", "Incorrect Keymap Syntax, no button where it was expected, instead I got: " + keymapFileStrings[iterator]);
			}

			iterator++;

			auto knit = buttonmapStringsToFunctions.find(keymapFileStrings[iterator]);
			if (knit != buttonmapStringsToFunctions.end())
			{
				newButtonFunction.buttonFunction = knit->second;
			}
			else
			{
				throwError("buttonmapSyntax", "Incorrect buttonmap Syntax, no function where it was expected, instead I got: " + keymapFileStrings[iterator]);
			}

			mButtonMap[newButton] = newButtonFunction;

			iterator++;
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
				if (mGUI != nullptr)
				{
					mGUI->keyDownFunc(evnt);
				}
				break;
			case SDL_KEYUP:
				unpressKey(evnt.key.keysym.sym);
				if (mGUI != nullptr)
				{
					mGUI->keyUpFunc(evnt);
				}
				break;
			case SDL_MOUSEMOTION:
				if (mGUI != nullptr)
				{
					mGUI->mouseMotionFunc(evnt);
				}
				break;
			case SDL_TEXTINPUT:
				if (mGUI != nullptr)
				{
					mGUI->decodeInputText(evnt);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				pressButton(evnt.button.button, glm::vec2(evnt.button.x, evnt.button.y));
				if (mGUI != nullptr)
				{
					mGUI->mouseButtonDownFunc(evnt);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				unpressButton(evnt.button.button, glm::vec2(evnt.button.x, evnt.button.y));
				if (mGUI != nullptr)
				{
					mGUI->mouseButtonUPFunc(evnt);
				}
				break;
			}
		}
	}

}