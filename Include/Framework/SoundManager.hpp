#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <fmod.hpp>
#include <map>

class Sound;

typedef std::map<const char*, Sound*> SoundMap;

class SoundManager
{
public:
	SoundManager();
	~SoundManager() throw();

	void Update();
	void PlaySound(Sound* soundToPlay);
	Sound* CreateSound(const char* filename);
	
	static SoundManager& GetInstance();

private:
	FMOD::System*					mSystem;
	SoundMap						mSounds;

	static SoundManager*			sInstance;
};

class Sound
{
public:
	friend class SoundManager;

	~Sound() throw();

private:
	FMOD::Sound*	mSound;

	Sound(const char* filename, FMOD::System* soundSystem);
	FMOD::Sound* GetSound();
};
#endif