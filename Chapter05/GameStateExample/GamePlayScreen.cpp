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
#include <bullet/btBulletDynamicsCommon.h>

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
	shaderManager.LinkShaders();

	// Get a handle for our "MVP" a.k.a ModelViewProjection uniform
	MatrixID = shaderManager.GetUniformLocation("ModelViewProjection");

	//Init Camera
	m_camera.Init();

	//Init Model
	m_model.Init("Meshes/Dwarf_2_Low.obj", "Textures/dwarf_2_1K_color.png");

	m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));

	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void GameplayScreen::OnExit()
{
	//glDeleteVertexArrays(1, &VertexArrayID);
}

void GameplayScreen::Update(float deltaTime)
{
	CheckInput(deltaTime);
	m_camera.Update();
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

}

int GameplayScreen::GetNextScreenIndex() const
{
	return SCREENINDEX_NO_SCREEN;
}

int GameplayScreen::GetPreviousScreenIndex() const
{
	return SCREENINDEX_NO_SCREEN;
}

void GameplayScreen::CheckInput(float deltaTime)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		m_game->OnSDLEvent(event);
	}

	// Get mouse position
	// Compute new orientation
	m_camera.SetHorizontalAngle(((1024.0f / 2.0f) - m_game->GetInputManager().GetMouseCoords().x) / 100.0f);
	m_camera.SetVerticalAngle(((768.0f / 2.0f) - m_game->GetInputManager().GetMouseCoords().y) / 100.0f); 

	if (m_game->GetInputManager().isKeyDown(SDLK_w)) {
		m_camera.MoveCamera(m_camera.GetForward() / deltaTime);
	}
	// Move backward
	if (m_game->GetInputManager().isKeyDown(SDLK_s)) {
		m_camera.MoveCamera(-m_camera.GetForward() / deltaTime);
	}
	// Strafe right
	if (m_game->GetInputManager().isKeyDown(SDLK_d)) {
		m_camera.MoveCamera(m_camera.GetRight() / deltaTime);
	}
	// Strafe left
	if (m_game->GetInputManager().isKeyDown(SDLK_a)) {
		m_camera.MoveCamera(-m_camera.GetRight() / deltaTime);
	}
}
