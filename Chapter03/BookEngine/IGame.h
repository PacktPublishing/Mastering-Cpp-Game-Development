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
#include <memory>
#include "BookEngine.h"
#include "Window.h"
#include "InputManager.h"
#include "ScreenList.h"


namespace BookEngine
{
	class IScreen;

	class IGame
	{
	public:
		IGame();
		virtual ~IGame();

		void Run();
		void ExitGame();

		virtual void OnInit() = 0;
		virtual void OnExit() = 0;
		
		virtual void AddScreens() = 0;
		
		void OnSDLEvent(SDL_Event& event);

		const float GetFPS() const
		{
			return m_fps;
		}

	protected:
		bool Init();

		virtual void Update();
		virtual void Draw();

		

		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IScreen* m_currentScreen = nullptr;
		Window m_window;
		InputManager m_inputManager;
		bool m_isRunning = false;
		float m_fps = 0.0f;
	};
}

