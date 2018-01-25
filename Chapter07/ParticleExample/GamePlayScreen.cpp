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
#include <BookEngine/GLTexture.h>
#include <BookEngine/ResourceManager.h>

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
	shaderManager.AddAttribute("VertexInitVel");
	shaderManager.AddAttribute("StartTime");
	//shaderManager.AddAttribute("vertexNormal");
	shaderManager.LinkShaders();

	// Get a handle for our "MVP" a.k.a ModelViewProjection uniform
	MatrixID = shaderManager.GetUniformLocation("ModelViewProjection");

	//Init Camera
	m_camera.Init();

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the point size
	glPointSize(15.0f);

	angle = glm::half_pi<float>();

	// Generate our vertex buffers
	InitBuffers();

	// Bind our texture in Texture Unit 0
	// Load Texture
	BookEngine::GLTexture m_texture = BookEngine::ResourceManager::GetTexture("Textures/bluewater.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture.id);

	GLint ParticleLifetime = shaderManager.GetUniformLocation("ParticleLifetime");
	glUniform1f(ParticleLifetime, 0);

	GLint Gravity = shaderManager.GetUniformLocation("Material.Ks");
	glUniform3f(Gravity, 0.0f, -0.2f, 0.0f);

	m_camera.SetView(glm::lookAt(glm::vec3(3.0f * cos(angle), 1.5f, 3.0f * sin(angle)), glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	//Init Model
	//m_model.Init("Meshes/Dwarf_2_Low.obj", "Textures/dwarf_2_1K_color.png");

	m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));
}

void GameplayScreen::OnExit()
{
	//glDeleteVertexArrays(1, &VertexArrayID);
}

void GameplayScreen::Update(float deltaTime)
{
	time = deltaTime;
	m_camera.Update();
	CheckInput();
}

void GameplayScreen::Draw()
{	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//Use shader
	shaderManager.Use();
	m_camera.SetView(glm::lookAt(glm::vec3(3.0f * cos(angle), 1.5f, 3.0f * sin(angle)), glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	
	GLint Time = shaderManager.GetUniformLocation("Time");
	glUniform1f(Time,time);
	
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_camera.GetMVPMatrix()[0][0]);

	//GLint textureUniform = shaderManager.GetUniformLocation("TextureSampler");
	//glUniform1i(textureUniform, 0);
	glBindVertexArray(particles);
	glDrawArrays(GL_POINTS, 0, nParticles);
	//Draw Model
	//m_model.Draw();
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

float GameplayScreen::RandFloat()
{
	return ((float)rand() / RAND_MAX);
}

void GameplayScreen::InitBuffers()
{

	nParticles = 8000;

	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

								 // Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, RandFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), RandFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(1.25f, 1.5f, RandFloat());
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &particles);
	glBindVertexArray(particles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
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
	//m_camera.SetHorizontalAngle((float(1024 / 2) - m_game->GetInputManager().GetMouseCoords().x) / 100);// * m_deltaTime; //m_mouseSpeed;
	//m_camera.SetVerticalAngle((float(768 / 2) - m_game->GetInputManager().GetMouseCoords().y) / 100); //* m_deltaTime;

	//m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));
	// Move forward
	if (m_game->GetInputManager().isKeyDown(SDLK_w)) {
		//m_camera.MoveCamera(m_camera.GetForward());//*m_deltaTime;
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
