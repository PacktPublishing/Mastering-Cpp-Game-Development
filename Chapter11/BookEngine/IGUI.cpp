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


#include "IGUI.h"
#include <algorithm>

namespace BookEngine {

	IGUI::IGUI(Window& window) : m_window(window)
	{
		m_camera = std::make_unique<Camera2D>();
		m_BL = new glm::vec2(0, 0);
		m_BR = new glm::vec2(m_window.GetScreenWidth(), 0);
		m_TL = new glm::vec2(0, m_window.GetScreenHeight());
		m_TR = new glm::vec2(m_window.GetScreenWidth(), m_window.GetScreenHeight());
		m_C = new glm::vec2(m_window.GetScreenWidth() * 0.5f, m_window.GetScreenHeight() * 0.5f);

		m_textureProgram = std::make_unique<BookEngine::ShaderManager>();
		m_spriteBatch = std::make_unique<BookEngine::SpriteBatch>();

	}
	


	IGUI::~IGUI()
	{
		std::for_each(m_GUIPanelsList.begin(), m_GUIPanelsList.end(), std::default_delete<IGUIPanel>());
		delete m_TL;
		delete m_TR;
		delete m_BR;
		delete m_BL;
		delete m_C;	
	}

	void IGUI::AddGUIObject(IGUIObject& GUIObject)
	{
		if (!m_GUIPanelsList.empty())
		{
			m_GUIPanelsList[0]->AddGUIObject(GUIObject);
		}
		else
		{
			IGUIPanel* panel = new IGUIPanel();
			m_GUIPanelsList.push_back(panel);
			m_GUIPanelsList[0]->AddGUIObject(GUIObject);
		}
	}
	void IGUI::AddGUIObject(IGUIObject& GUIObject, IGUIPanel& GUIPanel)
	{

	}
	void IGUI::AddGUIPanel(IGUIPanel& GUIPanel)
	{
		m_GUIPanelsList.push_back(&GUIPanel);
	}

	void IGUI::CheckInput(InputManager inputManager)
	{
		float pointX = inputManager.GetMouseCoords().x;
		float pointY = inputManager.GetMouseCoords().y;

		for (auto &panel : m_GUIPanelsList) // access by reference to avoid copying
		{
			for (auto& object : panel->GetGUIObjectList())
			{
				//Convert Y coordinate position to top upper left origin, y-down 
				float convertedY = m_window.GetScreenHeight() - (object->GetPosition().y + object->GetBounds().w);

				if (pointX < object->GetPosition().x + (object->GetBounds().z) &&
					pointX >(object->GetPosition().x - (object->GetBounds().z)) &&
					pointY < convertedY + object->GetBounds().w &&
					pointY > convertedY - object->GetBounds().w)
				{
					object->OnNotify(*object, GUIEvent::HoverOver);
					if (inputManager.isKeyPressed(SDL_BUTTON_LEFT))
					{
						object->OnNotify(*object, GUIEvent::Clicked);
					}
				}
			}
		}
	}

	glm::vec2* IGUI::GetAnchorPos(GUIAnchorPos anchorPos, glm::vec4 bounds)
	{
		switch (anchorPos)
		{
		case(GUIAnchorPos::TopRight):
			m_TR->y -= bounds.w;
			m_TR->x -= bounds.z;
			return m_TR;
			break;
		case(GUIAnchorPos::TopLeft):
			m_TL->y -= bounds.w;
			return m_TL;
			break;
		case(GUIAnchorPos::BottomRight):
			m_BR->x -= bounds.z;
			return m_BR;
			break;
		case(GUIAnchorPos::BottomLeft):			
			return m_BL;
			break;
		case(GUIAnchorPos::Center):
			m_C->y -= bounds.w;
			return m_C;
			break;
		}
	}
	
}