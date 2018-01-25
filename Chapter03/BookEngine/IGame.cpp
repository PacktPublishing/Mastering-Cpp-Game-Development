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
#include "IGame.h"
#include "IScreen.h"
#include "ScreenList.h"
#include "Timing.h"


namespace BookEngine
{

	IGame::IGame()
	{
		m_screenList = std::make_unique<ScreenList>(this);
	}

	IGame::~IGame()
	{
	}
	void IGame::Run()
	{
		if (!Init())
			return;

		FPSLimiter fpsLimiter;
		fpsLimiter.SetMaxFPS(60.0f);

		m_isRunning = true;

		///Game Loop
		while (m_isRunning)
		{
			fpsLimiter.Begin();

			m_inputManager.Update();

			Update();
			Draw();

			m_fps = fpsLimiter.End();

			m_window.SwapBuffer();
		}
		
	}
	void IGame::ExitGame()
	{
		m_currentScreen->OnExit();

		if (m_screenList)
		{
			m_screenList->Destroy();
			m_screenList.reset(); //Free memory
		}

		m_isRunning = false;
	}
	bool IGame::Init()
	{
		BookEngine::Init();

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_window.Create("BookEngine", 1024, 780, 0);

		OnInit();

		AddScreens();
		m_currentScreen = m_screenList->GetCurrentScreen();
		m_currentScreen->OnEntry();
		m_currentScreen->Run();


		return true;
	}
	void IGame::Update()
	{
		if (m_currentScreen)
		{
			switch (m_currentScreen->GetScreenState())
			{
			case ScreenState::RUNNING:
				m_currentScreen->Update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->OnExit();
				m_currentScreen = m_screenList->MoveToNextScreen();

				if (m_currentScreen)
				{
					m_currentScreen->Run();
					m_currentScreen->OnEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->OnExit();
				m_currentScreen = m_screenList->MoveToPreviousScreen();

				if (m_currentScreen)
				{
					m_currentScreen->Run();
					m_currentScreen->OnEntry();
				}
				break;
			case ScreenState::EXIT_APP:
				ExitGame();
				break;
			default:
				break;
			}
		}
		else
		{
			//we have no screen so exit
			ExitGame();
		}
	}
	void IGame::Draw()
	{
		//For safety
		glViewport(0, 0, m_window.GetScreenWidth(), m_window.GetScreenHeight());
		
		//Check if we have a screen and that the screen is running
		if (m_currentScreen &&
			m_currentScreen->GetScreenState() == ScreenState::RUNNING)
		{
			m_currentScreen->Draw();
		}
	}
	void IGame::OnSDLEvent(SDL_Event & event)
	{
		switch (event.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.SetMouseCoords((float)event.motion.x, (float)event.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.KeyPress(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.KeyRelease(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.KeyPress(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.KeyRelease(event.button.button);
			break;
		}
	}
}
