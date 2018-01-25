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
#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <BookEngine/IGame.h>
#include <BookEngine/ResourceManager.h>


GameplayScreen::GameplayScreen(BookEngine::Window* window) : m_window(window)
{
}


GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::Build()
{
}

void GameplayScreen::Destroy()
{

}

void GameplayScreen::OnEntry()
{
	// Init camera
	m_camera.Init(m_window->GetScreenWidth(), m_window->GetScreenHeight());
	m_camera.SetScale(1.0f);
	// Point the camera to the center of the screen
	m_camera.SetPosition(glm::vec2(m_window->GetScreenWidth() / 2.0f, m_window->GetScreenHeight() / 2.0f));
	
	//Lazy Init 
	if (m_shaderManager == nullptr)
	{
		m_shaderManager = std::make_unique<BookEngine::ShaderManager>();
		m_shaderManager->CompileShaders("Shaders/Texture.vert", "Shaders/Texture.frag");
		m_shaderManager->AddAttribute("vertexPosition");
		m_shaderManager->AddAttribute("vertexColor");
		m_shaderManager->AddAttribute("vertexUV");
		m_shaderManager->LinkShaders();
	}
	// Load the texture
	m_sprite.Init((m_window->GetScreenWidth() / 2.0f) - 200.0f, 
				   (m_window->GetScreenHeight() / 2.0f) - 200.0f,
				   400.0f, 
				   400.0f,
				   "Textures/sun.png");
	
}

void GameplayScreen::OnExit()
{
}

void GameplayScreen::Update(float deltaTime)
{
	m_camera.Update();
	CheckInput();
}

void GameplayScreen::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	m_shaderManager->Use();

	// Upload texture uniform
	GLint textureUniform = m_shaderManager->GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Camera matrix
	glm::mat4 projectionMatrix = m_camera.GetCameraMatrix();
	GLint pUniform = m_shaderManager->GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_sprite.Draw();

	m_shaderManager->UnUse();
}

int GameplayScreen::GetNextScreenIndex() const
{
	return SCREENINDEX_NO_SCREEN;
}

int GameplayScreen::GetPreviousScreenIndex() const
{
	return SCREENINDEX_NO_SCREEN;
}

void GameplayScreen::CheckInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		m_game->OnSDLEvent(event);
	}
}
