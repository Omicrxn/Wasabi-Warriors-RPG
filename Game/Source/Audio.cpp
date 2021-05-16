#include "App.h"
#include "Audio.h"
#include "AssetsManager.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

AudioManager::AudioManager(AssetsManager* assetsManager) : Module()
{
	music = NULL;
	name.Create("audio");

	this->assetsManager = assetsManager;
}

// Destructor
AudioManager::~AudioManager()
{}

// Called before render is available
bool AudioManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME/4);

	return ret;
}

// Called before quitting
bool AudioManager::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool AudioManager::LoadState(pugi::xml_node& audio)
{
	this->ChangeMusicVolume(audio.attribute("musicVolume").as_int());
	this->ChangeFxVolume(audio.attribute("fxVolume").as_int());
	return true;
}

bool AudioManager::SaveState(pugi::xml_node& audio) const
{
	/* ---------- CHECKS IF THE Attribute WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	SString tempName = audio.attribute("musicVolume").name();
	if (tempName == "musicVolume")
	{
		// Attribute currentMap exists
		audio.attribute("musicVolume").set_value(this->GetMusicVolume());
	}
	else
	{
		// Attribute currentMap does not exist
		audio.append_attribute("musicVolume").set_value(this->GetMusicVolume());
	}

	tempName = audio.attribute("fxVolume").name();
	if (tempName == "fxVolume")
	{
		// Attribute currentMap exists
		audio.attribute("fxVolume").set_value(this->GetFxVolume());
	}
	else
	{
		// Attribute currentMap does not exist
		audio.append_attribute("fxVolume").set_value(this->GetFxVolume());
	}
	return true;
}

// Play a music file
bool AudioManager::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{

		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);

	}

	music = Mix_LoadMUS(path);
	//SDL_RWops* rw = assetsManager->LoadAsset(path);
	//music = Mix_LoadMUS_RW(rw, 0);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}

		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}

		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int AudioManager::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);
	//SDL_RWops* rw = assetsManager->LoadAsset(path);
	//Mix_Chunk* chunk = Mix_LoadWAV_RW(rw, 0);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	//SDL_RWclose(rw);

	return ret;
}

// Play WAV
bool AudioManager::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.Count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

bool AudioManager::ChangeMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
	return true;
}

bool AudioManager::ChangeFxVolume(int volume)
{
	ListItem<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
		Mix_VolumeChunk(item->data, volume);
	return false;
}

int AudioManager::GetMusicVolume() const
{
	return Mix_VolumeMusic(-1);
}

int AudioManager::GetFxVolume() const
{
	return Mix_VolumeChunk(fx.start->data, -1);
}

bool AudioManager::MuteMusic() 
{
	bool ret = true;
	//If the music is paused
	if (Mix_PausedMusic() == 1)
	{
		//Resume the music
		Mix_ResumeMusic();
	}
	//If the music is playing
	else
	{
		//Pause the music
		Mix_PauseMusic();
	}


	return ret;
}

bool AudioManager::StopMusic() 
{
	Mix_FreeMusic(music);
	music = nullptr;
	Mix_HaltMusic();
	return true;
}

bool AudioManager::UnloadFx(uint index)
{
	bool ret = false;

	/*Mix_FreeChunk(fx.At(index - 1)->data);
	fx.Del(fx.At(index - 1));*/

	ListItem<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
	{
		if (fx.At(index) == item)
		{
			Mix_FreeChunk(item->data);
			fx.Del(item);
			return true;
		}
	}
	RELEASE(item);

	return ret;
}