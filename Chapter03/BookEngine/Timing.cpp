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
#include "Timing.h"
#include <SDL/SDL.h>

namespace BookEngine
{
	FPSLimiter::FPSLimiter()
	{
	}

	void FPSLimiter::Init(float maxFPS)
	{
		SetMaxFPS(maxFPS);
	}
	void FPSLimiter::Begin()
	{
		m_startTicks = SDL_GetTicks();
	}

	void FPSLimiter::SetMaxFPS(float maxFPS)
	{
		m_maxFPS = maxFPS;
	}

	float FPSLimiter::End()
	{
		CalcFPS();
		float frameTicks = SDL_GetTicks() - m_startTicks;
		//Limit FPS to the maxFS
		if (1000.0f / m_maxFPS > frameTicks)
			SDL_Delay((Uint32)(1000.0f / m_maxFPS - frameTicks));

		return m_fps;
	}
	void FPSLimiter::CalcFPS()
	{
		//The number of frames to average
		static const int NUM_SAMPLES = 10;
		//Stores all the frametimes for each frame that we will average
		static float frameTimes[NUM_SAMPLES];
		//The current frame we are on
		static int currentFrame = 0;
		//the ticks of the previous frame
		static float prevTicks = SDL_GetTicks();

		//Ticks for the current frame
		float currentTicks = SDL_GetTicks();

		//Calculate the number of ticks (ms) for this frame
		m_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

		//current ticks is now previous ticks
		prevTicks = currentTicks;

		//The number of frames to average
		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES)
			count = currentFrame;
		else
			count = NUM_SAMPLES;

		//Average all the frame times
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
			frameTimeAverage += frameTimes[i];
		frameTimeAverage /= count;

		//Calculate FPS
		if (frameTimeAverage > 0)
			m_fps = 1000.0f / frameTimeAverage;
		else
			m_fps = 60.0f;

	}
}
