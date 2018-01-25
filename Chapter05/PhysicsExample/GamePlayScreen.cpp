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
#include <BookEngine/UnitConverter.h>


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
	//Init Camera
	m_camera.Init();

	m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));

	dynamicsWorld->setGravity(btVector3(0, -1, 0));

	m_ground.Init(glm::vec3(0.0f,-30.0f,0.0f));
	m_ball.Init(glm::vec3(0.0f, 10.0f, -20.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	dynamicsWorld->addRigidBody(m_ground.groundRigidBody);

	dynamicsWorld->addRigidBody(m_ball.fallRigidBody);

}

void GameplayScreen::OnExit()
{
	// Clean up behind ourselves like good little programmers
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}

void GameplayScreen::Update(float deltaTime)
{
	CheckInput(deltaTime);

	dynamicsWorld->stepSimulation(1 / 60.f, 10);

	btTransform trans;
	m_ball.fallRigidBody->getMotionState()->getWorldTransform(trans);
	m_ball.Update(deltaTime);

	std::cout << "sphere height: " << trans.getOrigin().getX() << trans.getOrigin().getY() << trans.getOrigin().getZ() << std::endl;
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
	
	m_ground.Draw(&m_camera);
	m_ball.Draw(&m_camera);

	UnitConverter Converter;
	Converter.Converter(new btVector3(), UnitConverter().Bullet);

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
