#include "OAudio.h"
#include<fstream>

namespace Otherwise
{
	void errorCheck(FMOD_RESULT result, std::string failLine)
	{
		if (result != FMOD_OK)
		{
			throwError("FMODError", failLine);
		}
	}

	OSound::OSound()
	{

	}

	OSound::~OSound()
	{

	}

	void OSound::init(CorrespondentManager * corrManager, std::string & soundFile, FMOD::System * system)
	{
		soundReciever.init(corrManager, soundFile);
		mSystem = system;
		errorCheck(mSystem->createSound(soundFile.c_str(), FMOD_DEFAULT, NULL, &mSound), "Sound " + soundFile + " could not be initialized");
	}

	void OSound::update()
	{
		if (soundReciever.getMessage())
		{
			errorCheck(mSystem->playSound(mSound, NULL, false, NULL), "Sound could not be played.");
			soundReciever.clearMessage();
		}
	}

	void OSound::destroy()
	{
		errorCheck(mSound->release(), "Failed to release Sound");
	}

	OAudio::OAudio()
	{
	}

	OAudio::~OAudio()
	{
	}

	void OAudio::init(std::string & initialSoundMapFilePath, CorrespondentManager * corrManager)
	{
		errorCheck(FMOD::System_Create(&mSystem), "System Failed to be created.");
		errorCheck(mSystem->init(8, FMOD_INIT_NORMAL, NULL), "FMOD System failed to initialize.");
		mCorrespondentManager = corrManager;
		loadSoundMap(initialSoundMapFilePath);
	}

	void OAudio::loadSoundMap(std::string & soundMapFilePath)
	{
		if (!mSoundMap.empty())
		{
			mSoundMap.clear();
		}

		std::string tempString;
		std::ifstream fileStream(soundMapFilePath);
		if (fileStream.is_open())
		{
			while (getline(fileStream, tempString))
			{
				if (tempString == "SOUND")
				{
					getline(fileStream, tempString);
					getline(fileStream, tempString);
					getline(fileStream, tempString);
					getline(fileStream, tempString);
					OSound newSound;
					mSoundMap.push_back(newSound);
					mSoundMap.back().init(mCorrespondentManager, tempString, mSystem);
				}
			}
		}
		else
		{
			throwError(soundMapFilePath, soundMapFilePath + " sound map file is missing or corrupted.");
		}
	}

	void OAudio::update()
	{
		for (unsigned int i = 0; i < mSoundMap.size(); i++)
		{
			mSoundMap[i].update();
		}
	}
}