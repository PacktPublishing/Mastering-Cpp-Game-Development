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

namespace BookEngine {

    Window::Window()
    {
    }


    Window::~Window()
    {
    }

    int Window::Create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

        Uint32 flags = SDL_WINDOW_OPENGL;
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        if (currentFlags & INVISIBLE) {
            flags |= SDL_WINDOW_HIDDEN;
        }
        if (currentFlags & FULLSCREEN) {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        if (currentFlags & BORDERLESS) {
            flags |= SDL_WINDOW_BORDERLESS;
        }

		if (currentFlags & RESIZABLE) {
			flags |= SDL_WINDOW_RESIZABLE;
		}
		if (currentFlags & FULLSCREEN_DESKTOP) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		

        //Open an SDL window
        m_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
        if (m_window == nullptr) {
            throw Exception("SDL Window could not be created!");
        }

        //Set up our OpenGL context
        SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
        if (glContext == nullptr) {
            throw Exception("SDL_GL context could not be created!");
        }

        //Set up glew (optional but recommended)
        GLenum error = glewInit();
        if (error != GLEW_OK) {
            throw Exception("Could not Initialize glew!");
        }

        //Check the OpenGL version
		std::string versionString = (char*)glGetString(GL_VERSION);
		std::string vendorString = (char*)glGetString(GL_VENDOR);
		std::string renderString = (char*)glGetString(GL_RENDERER);

		WriteLog(BookEngine::LogType::RUN, "***   OpenGL Version : " + versionString + " ***");
		WriteLog(BookEngine::LogType::RUN, "***   Chipset Vendor : " + vendorString + " ***");
		WriteLog(BookEngine::LogType::RUN, "***   Rendering Hardware : " + renderString + " ***");
	
        //Set the background color to blue
       // glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        //Set VSYNC
        SDL_GL_SetSwapInterval(0);

        // Enable alpha blend
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable Anti Aliasing
		glEnable(GL_MULTISAMPLE);

		//Set window flag
		m_isWindowed = true;

        return 0;
    }

    void Window::SwapBuffer() {
        SDL_GL_SwapWindow(m_window);
    }

	void Window::ToggleFullScreen()
	{
		////If the screen is windowed
		//if (m_isWindowed == true)
		//{
		//	//Set the screen to fullscreen
		//	m_window = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN);

		//	//Set the window state flag
		//	m_isWindowed = false;
		//}
		////If the screen is fullscreen
		//else if (m_isWindowed == false)
		//{
		//	//Window the screen
		//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);

		//	//Set the window state flag
		//	m_isWindowed = true;
		//}
	}

	void Window::OnWindowEvent(SDL_Event & evnt)
	{

		//If the window resized
		if (evnt.type == SDL_WINDOWEVENT_RESIZED)
		{
			//Resize the screen
			//m_window = SDL_SetVideoMode(evnt.resize.w, evnt.resize.h, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);
		}
	}

	void Window::SetWindowName(std::string & windowName)
	{
		SDL_SetWindowTitle(m_window, windowName.c_str());
	}

}