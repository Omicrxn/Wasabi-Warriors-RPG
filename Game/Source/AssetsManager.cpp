#include "AssetsManager.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "PhysFS/include/physfsrwops.h"
#include "PhysFS/include/physfsrwops.c"

AssetsManager::AssetsManager() : Module()
{
	name.Create("assetsManager");
}

// Destructor
AssetsManager::~AssetsManager()
{
}

// Called before Assets Manager is available
bool AssetsManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Assets Manager");
	bool ret = true;

	if (PHYSFS_init(NULL) == 0)
	{
		LOG("ERROR INITIALIZING PHYSFS LIBRARY: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	if (PHYSFS_mount("Assets.zip", NULL, 1) == 0)
	{
		LOG("ERROR ADDING ARCHIVE TO SEARCH PATH: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	return ret;
}

SDL_RWops* AssetsManager::LoadAsset(const char* path)
{
	if (PHYSFS_exists(path) == 0)
	{
		LOG("ERROR - FILE %s DOESNT EXIST IN THE SEARCH PATH: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return NULL;
	}

	SDL_RWops* ret = PHYSFSRWOPS_openRead(path);

	if (ret == NULL)
	{
		LOG("ERROR OPENING FILE %s FOR READING: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return NULL;
	}

	return ret;
}

size_t AssetsManager::LoadXML(const char* path, char** buffer)
{
	PHYSFS_sint64 numBytesRead = 0;

	if (PHYSFS_exists(path) == 0)
		LOG("ERROR - FILE %s DOESNT EXIST IN THE SEARCH PATH: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file == NULL)
		LOG("ERROR OPENING FILE %s FOR READING: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	if (!PHYSFS_eof(file))
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(file);

		*buffer = new char[size];

		numBytesRead = PHYSFS_readBytes(file, *buffer, size);
		if (numBytesRead == -1)
			LOG("ERROR READING FROM FILEHANDLE: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

		if (numBytesRead == size)
		{
			if (PHYSFS_close(file) == 0)
				LOG("ERROR CLOSING FILEHANDLE: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

			return numBytesRead;
		}
		else
		{
			PHYSFS_close(file);
			RELEASE_ARRAY(buffer);
			return 0;
		}
	}
	return numBytesRead;
}

// Called before quitting
bool AssetsManager::Cleanup()
{
	LOG("Quitting Assets Manager");

	if (PHYSFS_deinit() == 0)
		LOG("ERROR DEINITIALIZING THE PHYSFS LIBRARY: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return true;
}