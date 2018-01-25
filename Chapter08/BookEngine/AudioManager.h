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
#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace BookEngine
{

	class SoundEffect
	{
	public:
		friend class AudioManager;
		///Plays the sound file
		///@param numOfLoops: If == -1, loop forever,
		///otherwise loop of number times provided + 1
		void Play(int numOfLoops = 0);

	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music
	{
	public:
		friend class AudioManager;
		///Plays the music file
		///@param numOfLoops: If == -1, loop forever,
		///otherwise loop of number times provided
		void Play(int numOfLoops = -1);

		static void Pause() { Mix_PauseMusic(); };
		static void Stop() { Mix_HaltMusic(); };
		static void Resume() { Mix_ResumeMusic(); };

	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		void Init();
		void Destroy();

		SoundEffect LoadSoundEffect(const std::string& filePath);
		Music LoadMusic(const std::string& filePath);
	private:
		std::map<std::string, Mix_Chunk*> m_effectList;
		std::map<std::string, Mix_Music*> m_musicList;
		bool m_isInitialized = false;
	};
}

