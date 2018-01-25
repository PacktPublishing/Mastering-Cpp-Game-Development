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
#include <GL\glew.h>
#include "IGame.h"
#include "Timing.h"

#include "ScreenList.h"
#include "IScreen.h"

namespace BookEngine {

    IGame::IGame() {
        m_screenList = std::make_unique<ScreenList>(this);
		m_window = std::make_shared<Window>();
    }

    IGame::~IGame() {
        // Empty
    }

    void IGame::Run() {

        if (!Init()) return;

        FpsLimiter limiter;
        limiter.SetMaxFPS(60.0f);

        // Game loop
        m_isRunning = true;
        while (m_isRunning) {
            limiter.Begin();

            // Call the custom update and draw method
            m_inputManager.Update();
            Update();
            Draw();
			m_deltaTime = limiter.deltaTime;
            m_fps = limiter.End();
            m_window->SwapBuffer();
        }

    }

    void IGame::Exit() {
        m_currentScreen->OnExit();
        if (m_screenList) {
            m_screenList->Destroy();
            m_screenList.reset();
        }
        m_isRunning = false;
    }

    void IGame::OnSDLEvent(SDL_Event& evnt) {
        switch (evnt.type) {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.SetMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.KeyPress(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.KeyRelease(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.KeyPress(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.KeyRelease(evnt.button.button);
                break;
        }
    }

    bool IGame::Init() {
        BookEngine::Init();

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	
        if (!InitSystems()) return false;

        OnInit();
        AddScreens();

        m_currentScreen = m_screenList->GetCurrent();
        m_currentScreen->OnEntry();
        m_currentScreen->SetRunning();

        return true;
    }

    bool IGame::InitSystems() {
		SDL_DisplayMode target, closest;

		target.w = 1024;
		target.h = 768;
		target.format = 0;
		target.refresh_rate = 0; // don't care
		target.driverdata = 0; // initialize to 0


							   // Pass the display mode structures by reference to SDL_GetClosestDisplay
							   // and check whether the result is a null pointer.
		if (SDL_GetClosestDisplayMode(0, &target, &closest) == NULL)
		{
			// If the returned pointer is null, no match was found.
			printf("\nNo suitable display mode was found!\n\n");
			return false;
		}
		else
			m_window->Create("Example Code", closest.w, closest.h, 8);
		return true;
    }


    void IGame::Update() {
        if (m_currentScreen) {
            switch (m_currentScreen->GetState()) {
                case ScreenState::RUNNING:
                    m_currentScreen->Update(m_deltaTime);
                    break;
                case ScreenState::CHANGE_NEXT:
                    m_currentScreen->OnExit();
                    m_currentScreen = m_screenList->MoveNext();
                    if (m_currentScreen) {
                        m_currentScreen->SetRunning();
                        m_currentScreen->OnEntry();
                    }
                    break;
                case ScreenState::CHANGE_PREVIOUS:
                    m_currentScreen->OnExit();
                    m_currentScreen = m_screenList->MovePrevious();
                    if (m_currentScreen) {
                        m_currentScreen->SetRunning();
                        m_currentScreen->OnEntry();
                    }
                    break;
                case ScreenState::EXIT_APPLICATION:
                    Exit();
                    break;
                default:
                    break;
            }
        } else {
            Exit();
        }
    }

    void IGame::Draw() {
        glViewport(0, 0, m_window->GetScreenWidth(), m_window->GetScreenHeight());
        if (m_currentScreen && m_currentScreen->GetState() == ScreenState::RUNNING) {
            m_currentScreen->Draw();
        }
    }
}