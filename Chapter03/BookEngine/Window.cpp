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
#include "Window.h"
#include "Exception.h"
#include "Logger.h"

namespace BookEngine
{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::Create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}


		try {
			//Open an SDL window
			m_SDL_Window = SDL_CreateWindow(windowName.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				screenWidth,
				screenHeight,
				flags);
			if (m_SDL_Window == nullptr)
				throw Exception("SDL Window could not be created!");

			//Set up our OpenGL context
			SDL_GLContext glContext = SDL_GL_CreateContext(m_SDL_Window);

			if (glContext == nullptr)
				throw Exception("SDL_GL context could not be created!");

			//Set up GLEW (optional)
			GLenum error = glewInit();
			if (error != GLEW_OK)
				throw ("Could not initialize glew!");

			//print some log info
			std::string versionNumber = (const char*)glGetString(GL_VERSION);
			WriteLog(LogType::RUN, "*** OpenGL Version: " + versionNumber + "***");

			//Set the background color to blue
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

			//Enable VSYNC
			SDL_GL_SetSwapInterval(1);

			//Enable alpha blend
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		}
		catch (Exception e)
		{
			//Write Log
			WriteLog(LogType::ERROR, e.reason);
		}

		return 0;
	}

	void Window::SwapBuffer()
	{
		SDL_GL_SwapWindow(m_SDL_Window);
	}
}