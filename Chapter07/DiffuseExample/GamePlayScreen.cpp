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


GameplayScreen::GameplayScreen()
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
	std::cout << "OnEntry\n";
	
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	shaderManager.CompileShaders("Shaders/SimpleShader.vert", "Shaders/SimpleShader.frag");
	shaderManager.AddAttribute("vertexPosition_modelspace");
	shaderManager.AddAttribute("vertexColor");
	shaderManager.AddAttribute("vertexNormal");
	shaderManager.LinkShaders();

	// Get a handle for our "MVP" a.k.a ModelViewProjection uniform
	MatrixID = shaderManager.GetUniformLocation("ModelViewProjection");

	//Init Camera
	m_camera.Init();

	//Init Model
	m_model.Init("Meshes/Dwarf_2_Low.obj", "Textures/dwarf_2_1K_color.png");

	m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));
}

void GameplayScreen::OnExit()
{
	//glDeleteVertexArrays(1, &VertexArrayID);
}

void GameplayScreen::Update(float gameTime)
{
	
	m_camera.Update();
	CheckInput();
}

void GameplayScreen::Draw()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//Use shader
	shaderManager.Use();

	GLint Kd = shaderManager.GetUniformLocation("Kd");
	glUniform3f(Kd, 0.9f, 0.5f, 0.3f);
	GLint Ld = shaderManager.GetUniformLocation("Ld");
	glUniform3f(Ld, 1.0f, 1.0f, 1.0f);

	glm::vec4 lightPos = m_camera.GetView() * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);

	GLint lightPosUniform = shaderManager.GetUniformLocation("LightPosition");
	glUniform4f(lightPosUniform, lightPos[0], lightPos[1], lightPos[2], lightPos[3]);

	glm::mat4 modelView = m_camera.GetView() * glm::mat4(1.0f);
	GLint modelViewUniform = shaderManager.GetUniformLocation("ModelViewMatrix");
	glUniformMatrix4fv(modelViewUniform, 1, GL_FALSE, &modelView[0][0]);
	
	glm::mat3 normalMatrix = glm::mat3(glm::vec3(modelView[0]), glm::vec3(modelView[1]), glm::vec3(modelView[2]));
	GLint normalMatrixUniform = shaderManager.GetUniformLocation("NormalMatrix");
	glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, &normalMatrix[0][0]);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_camera.GetMVPMatrix()[0][0]);

	GLint textureUniform = shaderManager.GetUniformLocation("TextureSampler");
	glUniform1i(textureUniform, 0);

	//Draw Model
	m_model.Draw();
	//UnUse
	shaderManager.UnUse();
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

	// Get mouse position
	// Compute new orientation
	m_camera.SetHorizontalAngle((float(1024 / 2) - m_game->GetInputManager().GetMouseCoords().x) / 100);// * m_deltaTime; //m_mouseSpeed;
	m_camera.SetVerticalAngle((float(768 / 2) - m_game->GetInputManager().GetMouseCoords().y) / 100); //* m_deltaTime;

	//m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));
	// Move forward
	if (m_game->GetInputManager().isKeyDown(SDLK_w)) {
		m_camera.MoveCamera(m_camera.GetForward());//*m_deltaTime;
	}
	// Move backward
	if (m_game->GetInputManager().isKeyDown(SDLK_s)) {
		m_camera.MoveCamera(-m_camera.GetForward());//*m_deltaTime;
	}
	// Strafe right
	if (m_game->GetInputManager().isKeyDown(SDLK_d)) {
		m_camera.MoveCamera(m_camera.GetRight());// *m_deltaTime;
	}
	// Strafe left
	if (m_game->GetInputManager().isKeyDown(SDLK_a)) {
		m_camera.MoveCamera(-m_camera.GetRight());//*m_deltaTime;
	}
}
