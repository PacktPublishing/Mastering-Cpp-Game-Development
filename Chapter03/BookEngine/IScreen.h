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

#define SCREENINDEX_NO_SCREEN -1

namespace BookEngine 
{
	class IGame;

	enum class ScreenState
	{
		NONE,
		RUNNING,
		EXIT_APP,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IScreen
	{
	public:
		friend class ScreenList;

		IScreen()
		{
			
		};
		virtual ~IScreen() 
		{
			
		};

		//Called on beginning and end of app
		virtual void Build() = 0; //pure virtual (makes it a must to inherit)
		virtual void Destroy() = 0;

		virtual void OnEntry() = 0;
		virtual void OnExit() = 0;

		virtual void Update() = 0;
		virtual void Draw() = 0;

		void Run() 
		{
			m_currentState = ScreenState::RUNNING;
		}

		///Return the current screen index
		int GetScreenIndex() const
		{
			return m_screenIndex;
		}

		ScreenState GetScreenState() const
		{
			return m_currentState;
		}

		virtual int GetNextScreenIndex() const = 0;
		virtual int GetPreviousScreenIndex() const = 0;

		void SetParentGame(IGame* game)
		{
			m_game = game;
		}

	protected:

		ScreenState m_currentState = ScreenState::NONE;
		IGame* m_game = nullptr;
		int m_screenIndex = -1;
	
	private:

	};
}

