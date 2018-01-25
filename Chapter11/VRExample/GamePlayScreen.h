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
#include <BookEngine/SkyBox.h>

#include <openvr/openvr.h>
#include <openvr/ivrclientcore.h>
#include <openvr\vrcommon\dirtools_public.h>
#include <openvr\vrcommon\envvartools_public.h>
#include <openvr\vrcommon\hmderrors_public.h>
#include <openvr\vrcommon\pathtools_public.h>
#include <openvr\vrcommon\sharedlibtools_public.h>
#include <openvr\vrcommon\strtools_public.h>
#include <openvr\vrcommon\vrpathregistry_public.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <bullet/btBulletDynamicsCommon.h>

#include <GL\GL.h>

#include "GroundObject.h"


#include "DialogGUI.h"
#include "NPC.h"
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

	void Say(std::string stringToSay);
	
	
	GLuint MatrixID;
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint VertexArrayID;
	BookEngine::ShaderManager shaderManager;


	void HandleVRError(vr::EVRInitError err);
	void InitVR();
	std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL);
	void SubmitFramesOpenGL(GLint leftEyeTex, GLint rightEyeTex, bool linear = false);


	void ProcessVREvent(const vr::VREvent_t & event);
	void RenderFrame();
	bool SetupStereoRenderTargets();
	void SetupCameras();

	void RenderScene(vr::Hmd_Eye nEye);

	

private:
	void CheckInput(float deltaTime);
	BookEngine::Camera3D* m_camera;
	BookEngine::Mesh m_model;
	DialogGUI* m_gui;
	NPC m_npc1;

	BookEngine::SkyBox m_skybox;


	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	GroundObject m_ground;
	
	
	vr::IVRSystem* m_hmd;
	uint32_t m_VRWidth;
	uint32_t m_VRHeight;

	

};

