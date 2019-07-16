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

	void InputHandler::mapMouse(unsigned buttonID, std::string& signature)
	{
		auto newButton = std::make_unique<MouseOspondent>();
		newButton->corr.init(mCorrespondentManager, signature);
		newButton->cond.down = true;
		newButton->cond.button = buttonID;
		mButtons.push_back(std::move(newButton));
	}

	void InputHandler::init(CorrespondentManager *corrManager,
		OSInterface * osInterface)
	{
		mOSInterface = osInterface;
		mCorrespondentManager = corrManager;

		auto newMotion = std::make_unique<MotOspondent>();
		newMotion->corr.init(mCorrespondentManager,
			(std::string)"CameraLookAtSender");
		newMotion->relative = true;
		mMotions.push_back(std::move(newMotion));

		std::string tempString = "Init.txt";
		std::string tempStart = "InputHandler";
		std::string tempEnd = "InputHandlerEnd";
		std::vector<std::string> tempVector;
		extractLinesFromFile(&tempVector, tempStart, tempEnd, tempString);

		for (std::string i : tempVector)
		{
			findAllKeys(i);
		}
	}

	void InputHandler::findAllKeys(std::string & filePath)
	{
		std::string tempString;
		std::string currentKey;
		std::ifstream fileStream(filePath);
		if (fileStream.is_open())
		{
			while (getline(fileStream, tempString))
			{
				if (tempString == "KEY")
				{
					getline(fileStream, tempString);
					getline(fileStream, tempString);
					currentKey = tempString;
				}
				
				if (tempString == "KEYCODE")
				{
					getline(fileStream, tempString);
					mapKey(std::stoi(tempString, nullptr, 10), currentKey);
				}
			}
		}
		else
		{
			throwError(filePath, filePath + " could not be opened.");
		}
	}

	void InputHandler::mapKey(unsigned int keyID, std::string & signature)
	{
		auto newKey = std::make_unique<KeyOspondent>();
		newKey->corr.init(mCorrespondentManager, signature);
		newKey->cond.down = true;
		newKey->cond.key = keyID;
		mKeys.push_back(std::move(newKey));
	}

	void InputHandler::inputQueue()
	{
		InputState state = mOSInterface->getInput();

		for (unsigned int i = 0; i < state.mButtons.size(); i++)
		{
			for (unsigned int j = 0; j < mButtons.size(); j++)
			{
				mButtons[j]->update(&state.mButtons[i]);
			}
		}

		for (unsigned int i = 0; i < state.mKeys.size(); i++)
		{
			for (unsigned int j = 0; j < mKeys.size(); j++)
			{
				mKeys[j]->update(&state.mKeys[i]);
			}
		}

		for (unsigned int i = 0; i < state.mMotions.size(); i++)
		{
			for (unsigned int j = 0; j < mMotions.size(); j++)
			{
				mMotions[j]->update(&state.mMotions[i]);
			}
		}
	}

}