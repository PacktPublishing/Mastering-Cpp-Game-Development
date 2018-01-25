#include "DialogGUI.h"
#include <BookEngine/ResourceManager.h>
#include <algorithm>

DialogGUI::DialogGUI(BookEngine::Window& window) : IGUI(window)
{
	
}


DialogGUI::~DialogGUI()
{
}

void DialogGUI::OnInit()
{
	m_textureProgram->CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram->AddAttribute("vertexPosition");
	m_textureProgram->AddAttribute("vertexColor");
	m_textureProgram->AddAttribute("vertexUV");
	m_textureProgram->LinkShaders();

	m_spriteBatch->Init();

	m_camera->Init(m_window.GetScreenWidth(), m_window.GetScreenHeight());
	m_camera->SetPosition(glm::vec2(m_window.GetScreenWidth() * 0.5f, m_window.GetScreenHeight()* 0.5f));

	

	m_texture = new BookEngine::GLTexture(BookEngine::ResourceManager::GetTexture("Textures/button.png"));

	m_panel = new BookEngine::IGUIPanel(glm::vec4(0, 0, 500, 150), *GetAnchorPos(BookEngine::GUIAnchorPos::TopLeft, glm::vec4(0, 0, 500, 250)), glm::vec2(0, 0));
	AddGUIPanel(*m_panel);
	
	

	
	
}

void DialogGUI::OnExit()
{
	
}

void DialogGUI::SetDialog(std::string text)
{
	m_dialog = new BookEngine::IGUILabel(glm::vec4(0, 110, 250, 30),
		glm::vec2(110, -10),
		text,
		new BookEngine::SpriteFont("Fonts/Impact_Regular.ttf", 72),
		glm::vec2(0.3f), m_panel);

	AddGUIObject(*m_dialog);
	//m_dialog->SetLabelText(text);
}

void DialogGUI::SetOption(std::string text, int choiceNumber)
{
	choices.resize(choices.size() + 1);

	choices[choiceNumber] = new BookEngine::IGUILabel(glm::vec4(0, 110, 250, 20),
			glm::vec2(110, 10),
			text,
			new BookEngine::SpriteFont("Fonts/Impact_Regular.ttf", 72),
			glm::vec2(0.3f), m_panel);

	AddGUIObject(*choices[choiceNumber]);
	
}

void DialogGUI::RemoveAllPanelObjects()
{
	m_panel->RemoveAllGUIObjects();
}


void DialogGUI::Update()
{
	//Update Camera
	m_camera->Update();

	//Update Objects
	for (auto const& value : m_GUIPanelsList)
	{
		value->Update();
	}

}

void DialogGUI::Draw()
{

	glDepthMask(GL_TRUE);
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram->Use();

	// Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram->GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera->GetCameraMatrix();
	GLint pUniform = m_textureProgram->GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch->Begin();

	//Draw all objects
	for (auto const& value : m_GUIPanelsList)
	{
		value->Draw(*m_spriteBatch);
	}


	m_spriteBatch->End();
	m_spriteBatch->BatchRender();
	m_textureProgram->UnUse();

}




