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
#include "ScreenList.h"
#include "IScreen.h"

namespace BookEngine
{

	ScreenList::ScreenList(IGame * game) :
		m_game(game)
	{
		//Empty
	}

	ScreenList::~ScreenList()
	{
		Destroy();
	}

	
	IScreen * ScreenList::MoveToNextScreen()
	{
		IScreen* currentScreen = GetCurrentScreen();

		if (currentScreen->GetNextScreenIndex() != SCREENINDEX_NO_SCREEN)
		{
			m_currentScreenIndex = currentScreen->GetNextScreenIndex();
		}

		return GetCurrentScreen();
	}

	IScreen * ScreenList::MoveToPreviousScreen()
	{
		IScreen* currentScreen = GetCurrentScreen();

		if (currentScreen->GetPreviousScreenIndex() != SCREENINDEX_NO_SCREEN)
		{
			m_currentScreenIndex = currentScreen->GetPreviousScreenIndex();
		}

		return GetCurrentScreen();
	}

	IScreen* ScreenList::GetCurrentScreen()
	{
		if (m_currentScreenIndex == SCREENINDEX_NO_SCREEN)
		{
			//Error
			return nullptr;
		}
		return m_screens[m_currentScreenIndex];
	}

	void ScreenList::SetCurrentScreen(int nextScreen)
	{
		m_currentScreenIndex = nextScreen;
	}

	void ScreenList::AddScreen(IScreen * newScreen)
	{
		newScreen->m_screenIndex = m_screens.size(); //set index correctly
		m_screens.push_back(newScreen);
		newScreen->Build();
		newScreen->SetParentGame(m_game);
	}

	void ScreenList::Destroy()
	{
		for (size_t i = 0; i < m_screens.size(); i++)
		{
			m_screens[i]->Destroy();
		}
		m_screens.resize(0);
		m_currentScreenIndex = SCREENINDEX_NO_SCREEN;
	}

}
