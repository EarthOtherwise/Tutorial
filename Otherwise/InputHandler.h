#pragma once
#include<unordered_map>
#include "ErrHandler.h"
#include "OSInterface.h"
#include "glm/glm.hpp"
#include "MessagingSystem.h"

namespace Otherwise
{

	struct KeyOspondent
	{
		void update(KeyState * state)
		{
			if (cond.keyConditions(state))
			{
				corr.publish();
			}
		}
		KeyCondition cond;
		Correspondent corr;
	};

	struct MouseOspondent
	{
		void update(MouseState * state)
		{
			if (cond.mouseConditions(state))
			{
				corr.publish(glm::vec2(static_cast<float>(state->x),
					static_cast<float>(state->y)));
			}
		}
		MouseCondition cond;
		Correspondent corr;
	};

	struct MotOspondent
	{
		void update(MouseMotion * motion)
		{
			if(relative)
			{
				corr.publish(glm::vec2(static_cast<float>(motion->xRel),
					static_cast<float>(motion->yRel)));
			}
			else
			{
				corr.publish(glm::vec2(static_cast<float>(motion->x),
					static_cast<float>(motion->y)));
			}
		}
		bool relative = false;
		Correspondent corr;
	};
	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		void init(CorrespondentManager *corrManager, OSInterface * osInterface);

		void findAllKeys(std::string &filePath);

		void mapKey(unsigned int keyID, std::string &signature);

		void mapMouse(unsigned int buttonID, std::string &signature);

		/*Logs keystrokes to be used by the game.*/
		void inputQueue();

	private:
		OSInterface * mOSInterface;
		std::vector<std::unique_ptr<KeyOspondent>> mKeys;
		std::vector<std::unique_ptr<MouseOspondent>> mButtons;
		std::vector<std::unique_ptr<MotOspondent>> mMotions;
		CorrespondentManager *mCorrespondentManager;
	};

}