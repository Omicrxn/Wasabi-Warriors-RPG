#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#include "List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class AssetsManager;

class AudioManager : public Module
{
public:

	AudioManager(AssetsManager* assetsManager);

	// Destructor
	virtual ~AudioManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);
	bool UnloadFx(uint index);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool ChangeMusicVolume(int volume);

	bool ChangeFxVolume(int volume);

	int GetMusicVolume() const;

	int GetFxVolume() const;

	bool MuteMusic();

	bool StopMusic();

private:

	AssetsManager* assetsManager;

	_Mix_Music* music;
	List<Mix_Chunk *> fx;
};

#endif // __AUDIO_H__