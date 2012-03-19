#include "SoundManager.hpp"
#include <fmod_errors.h>
#include <cassert>
#include <iterator>

SoundManager*		SoundManager::sInstance		= NULL;

SoundManager::SoundManager()
{
	assert(sInstance == 0);
	sInstance = this;

	FMOD_RESULT result;

	result = FMOD::System_Create(&mSystem);		// Create the main system object.
	if (result != FMOD_OK)
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));

	result = mSystem->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	if (result != FMOD_OK)
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

SoundManager::~SoundManager() throw()
{
	for(SoundMap::iterator it = mSounds.begin(); it != mSounds.end(); it++)
	{
		delete it->second;
	}

	mSystem->release();
	sInstance = NULL;
}

void SoundManager::Update()
{
	mSystem->update();
}

void SoundManager::PlaySound(Sound* soundToPlay)
{
	FMOD_RESULT result;

	result = mSystem->playSound(FMOD_CHANNEL_FREE, soundToPlay->GetSound(), false, NULL);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

Sound* SoundManager::CreateSound(const char* filename)
{
	Sound* newSound = NULL;

	SoundMap::iterator it = mSounds.find(filename);
	if(it == mSounds.end())
	{
		newSound = new Sound(filename, mSystem);
		mSounds[filename] = newSound;
	}
	else
		newSound = it->second;

	return newSound;
}

SoundManager& SoundManager::GetInstance()
{
	return *sInstance;
}

Sound::Sound(const char* filename, FMOD::System* soundSystem)
{
	FMOD_RESULT result;

	result = soundSystem->createSound(filename, FMOD_DEFAULT, 0, &mSound);		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
}

Sound::~Sound() throw()
{
	mSound->release();
}

FMOD::Sound* Sound::GetSound()
{
	return mSound;
}