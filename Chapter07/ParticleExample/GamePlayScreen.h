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
#include <BookEngine/IScreen.h>
#include <BookEngine/ShaderManager.h>
#include <BookEngine/Camera3D.h>
#include <BookEngine/Mesh.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameplayScreen : public BookEngine::IScreen
{
public:
	GameplayScreen();
	~GameplayScreen();

	// Inherited via IGameScreen
	virtual void Build() override;

	virtual void Destroy() override;

	virtual void OnEntry() override;

	virtual void OnExit() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	virtual int GetNextScreenIndex() const override;

	virtual int GetPreviousScreenIndex() const override;

	
	

	GLuint MatrixID;
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint VertexArrayID;
	BookEngine::ShaderManager shaderManager;

	float angle;
	float time;

	float RandFloat();
	void InitBuffers();

	GLuint initVel, startTime, particles;
	GLuint nParticles;

	//VBOPlane *plane;

private:
	void CheckInput();
	BookEngine::Camera3D m_camera;
	BookEngine::Mesh m_model;

};

