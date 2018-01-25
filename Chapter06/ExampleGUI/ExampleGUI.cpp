#include "ExampleGUI.h"
#include <BookEngine/ResourceManager.h>
#include <algorithm>

ExampleGUI::ExampleGUI(BookEngine::Window& window): IGUI(window)
{
	
}


ExampleGUI::~ExampleGUI()
{
}

void ExampleGUI::OnInit()
{
	m_textureProgram->CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram->AddAttribute("vertexPosition");
	m_textureProgram->AddAttribute("vertexColor");
	m_textureProgram->AddAttribute("vertexUV");
	m_textureProgram->LinkShaders();

	m_spriteBatch->Init();

	m_camera->Init(m_window.GetScreenWidth(), m_window.GetScreenHeight());
	m_camera->SetPosition(glm::vec2(m_window.GetScreenWidth() * 0.5f, m_window.GetScreenHeight()* 0.5f));

	BookEngine::GLTexture texture;

	texture = BookEngine::ResourceManager::GetTexture("Textures/button.png");

	panel = new BookEngine::IGUIPanel(glm::vec4(0, 0, 150, 500), *GetAnchorPos(BookEngine::GUIAnchorPos::BottomRight, glm::vec4(0, 0, 150, 500)), glm::vec2(0,0));
	AddGUIPanel(*panel);
	button = new BookEngine::IGUIButton(glm::vec4(0, 0, 100, 50), 
		glm::vec2(10, -10), 
		"My Button", 
		&texture, 
		new BookEngine::SpriteFont("Fonts/Impact_Regular.ttf", 72),
		glm::vec2(0.2f), panel);

	AddGUIObject(*button);

	BookEngine::IGUIButton* button2  = new BookEngine::IGUIButton(glm::vec4(0, 110, 100, 50),
		glm::vec2(10, -10),
		"My Button2",
		&texture,
		new BookEngine::SpriteFont("Fonts/Impact_Regular.ttf", 72),
		glm::vec2(0.2f), panel);

	//dGUIPanel(*panel);
	
	AddGUIObject(*button2);
}

void ExampleGUI::OnExit()
{
	delete panel;
	delete button;
	//delete button2;
}


void ExampleGUI::Update()
{
	//Update Camera
    m_camera->Update();

	//Update Objects
	for (auto const& value : m_GUIPanelsList)
	{
		value->Update();
	}
	
	
}

void ExampleGUI::Draw()
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
