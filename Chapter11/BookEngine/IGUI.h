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

#include "BookEngine.h"
#include "Window.h"
#include "IGUIPanel.h"
#include "Camera2D.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "InputManager.h"
#include "ShaderManager.h"

namespace BookEngine {

	class IGUI
	{
	public:

		IGUI(Window& window);
		virtual ~IGUI();

		virtual void OnInit() = 0;
		virtual void AddGUIObject(IGUIObject& GUIObject);
		virtual void AddGUIObject(IGUIObject& GUIObject, IGUIPanel& GUIPanel);
		virtual void AddGUIPanel(IGUIPanel& GUIPanel);
		virtual void OnExit() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void CheckInput(InputManager inputManager);
	
	protected:

		glm::vec2 * GetAnchorPos(GUIAnchorPos anchorPos, glm::vec4 bounds);
		Window& m_window;
		std::vector<IGUIPanel*> m_GUIPanelsList;
		std::unique_ptr<Camera2D> m_camera = nullptr;

		std::unique_ptr<ShaderManager> m_textureProgram = nullptr;
		std::unique_ptr<SpriteBatch> m_spriteBatch = nullptr;
	
	private:
		glm::vec2* m_TR;
		glm::vec2* m_TL;
		glm::vec2* m_BR;
		glm::vec2* m_BL;
		glm::vec2* m_C;
	};
}
