#pragma once
#include "FMOD/fmod.hpp"
#include "ErrHandler.h"
#include <string>
#include <map>

namespace Otherwise
{
	class OAudio
	{
	public:
		OAudio();
		~OAudio();

		void init();
		void errorCheck(FMOD_RESULT result, std::string failLine);
		void loadSound(std::string& soundFilePath, bool streaming = false);
		void oPlaySound(std::string & soundFilePath);
		void unLoadSound(std::string& soundFilePath);

	private:
		FMOD::System* mSystem = nullptr;
		std::map<std::string, FMOD::Sound*> mSoundMap;
	};

}