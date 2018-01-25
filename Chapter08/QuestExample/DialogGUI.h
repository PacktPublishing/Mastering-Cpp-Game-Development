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

#include <BookEngine/IGUI.h>
#include <BookEngine/ShaderManager.h>
#include <BookEngine/IGUIButton.h>
#include <BookEngine/IGUILabel.h>

#pragma once
class DialogGUI :
	public BookEngine::IGUI
{
public:
	DialogGUI(BookEngine::Window& window);
	~DialogGUI();

	virtual void OnInit() override;
	virtual void OnExit() override;

	void SetDialog(std::string text);
	void SetOption(std::string text, int choiceNumber);
	void Update() override;
	void Draw() override;
	//void DialogGUI::CheckInput(BookEngine::InputManager inputManager) override;
	//void RegisterListingNPC(NPC* npc);
	std::vector<BookEngine::IGUILabel*> choices;
	void RemoveAllPanelObjects();

private:

	BookEngine::IGUIPanel* m_panel;
	BookEngine::GLTexture* m_texture;
	BookEngine::IGUILabel* m_dialog;
	//std::vector<BookEngine::IGUILabel*> m_choices;
	//NPC* m_currentListingNPC;
	
};

