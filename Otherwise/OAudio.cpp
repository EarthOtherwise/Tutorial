#include "OAudio.h"

namespace Otherwise
{

	OAudio::OAudio()
	{
	}


	OAudio::~OAudio()
	{
	}
	void OAudio::init()
	{
		errorCheck(FMOD::System_Create(&mSystem), "System Failed to be created.");
		errorCheck(mSystem->init(8, FMOD_INIT_NORMAL, NULL), "FMOD System failed to initialize.");
	}
	void OAudio::errorCheck(FMOD_RESULT result, std::string failLine)
	{
		if (result != FMOD_OK)
		{
			throwError("FMODError", failLine);
		}
	}

	void OAudio::loadSound(std::string & soundFilePath, bool streaming/* = false*/)
	{
		auto doesSoundExist = mSoundMap.find(soundFilePath);
		if (doesSoundExist != mSoundMap.end())
		{
			return;
		}

		FMOD_MODE mode = FMOD_DEFAULT;
		if (streaming)
		{
			mode |= FMOD_CREATESTREAM;
		}

		FMOD::Sound* newSound;
		mSystem->createSound(soundFilePath.c_str(), mode, NULL, &newSound);
		mSoundMap[soundFilePath] = newSound;
	}

	void OAudio::oPlaySound(std::string & soundFilePath)
	{
		auto doesSoundExist = mSoundMap.find(soundFilePath);
		if (doesSoundExist != mSoundMap.end())
		{
			loadSound(soundFilePath);
		}
		mSystem->playSound(doesSoundExist->second, NULL, false, NULL);
	}

	void OAudio::unLoadSound(std::string & soundFilePath)
	{
		auto doesSoundExist = mSoundMap.find(soundFilePath);
		if (doesSoundExist != mSoundMap.end())
		{
			return;
		}
		errorCheck(doesSoundExist->second->release(), "Failed to release sound");
		mSoundMap.erase(doesSoundExist);
	}
}