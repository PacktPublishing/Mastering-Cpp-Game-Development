/*
    Copyright (c) 2016 Michael "Mickey" MacDonald.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "AudioManager.h"
#include "Exception.h"
#include "Logger.h"

namespace BookEngine
{

	AudioManager::AudioManager()
	{
	}


	AudioManager::~AudioManager()
	{
		Destroy();
	}

	void BookEngine::AudioManager::Init()
	{
		//Check if we have already been Initialized
		if (m_isInitialized)
			throw Exception("Audio manager is already Initialized");

		//Can be Bitwise combination of 
		//MIX_Init_FAC, MIX_Init_MOD, MIX_Init_MP3, MIX_Init_OGG
		if(Mix_Init(MIX_INIT_OGG || MIX_INIT_MP3) == -1)
			throw Exception("SDL_Mixer could not Initialize! Error: " + std::string(Mix_GetError()));

		if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
			throw Exception("Mix_OpenAudio Error: " + std::string(Mix_GetError()));

		m_isInitialized = true;
	}

	void BookEngine::AudioManager::Destroy()
	{
		if (m_isInitialized)
		{
			m_isInitialized = false;

			//Release the audio resources

			for(auto& iter : m_effectList)
				Mix_FreeChunk(iter.second);
			for(auto& iter : m_musicList)
				Mix_FreeMusic(iter.second);
			Mix_CloseAudio();
			Mix_Quit();

		}
	}

	SoundEffect BookEngine::AudioManager::LoadSoundEffect(const std::string & filePath)
	{
		SoundEffect effect;

		//Lookup audio file in the cached list
		auto iter = m_effectList.find(filePath);

		//Failed to find in cache, load
		if (iter == m_effectList.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			//Error Loading file
			if(chunk == nullptr)
				throw Exception("Mix_LoadWAV Error: " + std::string(Mix_GetError()));

			effect.m_chunk = chunk;
			m_effectList[filePath] = chunk;
		}
		else //Found in cache
		{
			effect.m_chunk = iter->second;
		}

		return effect;
	}

	Music BookEngine::AudioManager::LoadMusic(const std::string & filePath)
	{
		Music music;

		//Lookup audio file in the cached list
		auto iter = m_musicList.find(filePath);

		//Failed to find in cache, load
		if (iter == m_musicList.end())
		{
			Mix_Music* chunk = Mix_LoadMUS(filePath.c_str());
			//Error Loading file
			if (chunk == nullptr)
            {
                WriteLog(BookEngine::LogType::ERROR,"Mix_LoadMUS Error: " + std::string(Mix_GetError()));
				throw Exception("Mix_LoadMUS Error: " + std::string(Mix_GetError()));
                
            }

			music.m_music = chunk;
			m_musicList[filePath] = chunk;
		}
		else //Found in cache
		{
			music.m_music = iter->second;
		}

		return music;
	}
	void SoundEffect::Play(int numOfLoops)
	{
		//TODO: USE OLDEST CHANNEL THAT IS NOT IN USE FIRST
		//THIS WILL ERROR IF TOO MANY SOUNDS ARE ATTEMPTED AT ONCE  <-hack fix below
		if(Mix_PlayChannel(-1, m_chunk, numOfLoops) == -1)
			if (Mix_PlayChannel(0, m_chunk, numOfLoops) == -1)
				throw Exception("Mix_PlayChannel Error: " + std::string(Mix_GetError()));
	}
	
	void Music::Play(int numOfLoops)
	{
		if (Mix_PlayMusic(m_music, numOfLoops) == -1)
			throw Exception("Mix_PlayMusic Error: " + std::string(Mix_GetError()));
	}	
}