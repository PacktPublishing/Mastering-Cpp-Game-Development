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




GameplayScreen::GameplayScreen() : m_hmd(NULL)
{
	

}


GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::Build()
{
	if (!vr::VR_IsHmdPresent())
	{
		throw std::runtime_error("Error : HMD not detected on the system");
	}

	if (!vr::VR_IsRuntimeInstalled())
	{
		throw std::runtime_error("Error : OpenVR Runtime not detected on the system");
	}

	InitVR();

	if (!vr::VRCompositor())
	{
		throw std::runtime_error("Unable to initialize VR compositor!\n ");
	}

	m_hmd->GetRecommendedRenderTargetSize(&m_VRWidth, &m_VRHeight);

	std::cout << "Initialized HMD with suggested render target size : " << m_VRWidth << "x" << m_VRHeight << std::endl;
}
	

void GameplayScreen::Destroy()
{
	if (m_hmd)
	{
		vr::VR_Shutdown();
		m_hmd = NULL;
	}
}

void GameplayScreen::Say(std::string stringToSay)
{
	std::cout << stringToSay << std::endl;
}

void GameplayScreen::OnEntry()
{




	std::cout << "OnEntry\n";
	
	m_skybox.Init("Textures/TropicalSunnyDayFront2048.png","Textures/TropicalSunnyDayBack2048.png", "Textures/TropicalSunnyDayUp2048.png", "Textures/TropicalSunnyDayDown2048.png", "Textures/TropicalSunnyDayLeft2048.png", "Textures/TropicalSunnyDayRight2048.png",
		"Shaders/skybox.vert", "Shaders/skybox.frag");
	

	//Init Camera
	m_camera = new BookEngine::Camera3D();
	m_camera->Init();

	m_game->GetInputManager().SetMouseCoords(float(1024 / 2), float(768 / 2));

	dynamicsWorld->setGravity(btVector3(0, -1, 0));

	m_ground.Init(glm::vec3(0.0f, -30.0f, 0.0f), glm::vec3(1.0f));
	m_npc1.Init(glm::vec3(0.0f, 10.0f, -20.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	dynamicsWorld->addRigidBody(m_ground.groundRigidBody);

	dynamicsWorld->addRigidBody(m_npc1.fallRigidBody);
}

void GameplayScreen::OnExit()
{
	//glDeleteVertexArrays(1, &VertexArrayID);
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

	
	m_npc1.Update(deltaTime);

	m_camera->Update();

}

void GameplayScreen::Draw()
{
	
   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	//glEnable(GL_DEPTH_TEST); // enable depth-testing
	//glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_CULL_FACE);	// cull face
	//glCullFace(GL_BACK);		 // cull back face
	//glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front
	

	

	
	//// render a sky-box using the cube-map texture
	////glDisable(GL_DEPTH_TEST);
	//m_skybox.Draw(m_camera);
	////glClear(GL_DEPTH_BUFFER_BIT);
	////glEnable(GL_DEPTH_TEST);
	//m_ground.Draw(m_camera);
	//m_npc1.Draw(m_camera);

	
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
	m_camera->SetHorizontalAngle(((1024.0f / 2.0f) - m_game->GetInputManager().GetMouseCoords().x) / 100.0f);
	m_camera->SetVerticalAngle(((768.0f / 2.0f) - m_game->GetInputManager().GetMouseCoords().y) / 100.0f);

	if (m_game->GetInputManager().isKeyDown(SDLK_w)) {
		m_camera->MoveCamera(m_camera->GetForward() / deltaTime);
	}
	// Move backward
	if (m_game->GetInputManager().isKeyDown(SDLK_s)) {
		m_camera->MoveCamera(-m_camera->GetForward() / deltaTime);
	}
	// Strafe right
	if (m_game->GetInputManager().isKeyDown(SDLK_d)) {
		m_camera->MoveCamera(m_camera->GetRight() / deltaTime);
	}
	// Strafe left
	if (m_game->GetInputManager().isKeyDown(SDLK_a)) {
		m_camera->MoveCamera(-m_camera->GetRight() / deltaTime);
	}

	//m_gui->CheckInput(m_game->GetInputManager());


}

void GameplayScreen::HandleVRError(vr::EVRInitError err)
{
	throw std::runtime_error(vr::VR_GetVRInitErrorAsEnglishDescription(err));
}

void GameplayScreen::InitVR()
{
	vr::EVRInitError err = vr::VRInitError_None;
	m_hmd = vr::VR_Init(&err, vr::VRApplication_Scene);

	if (err != vr::VRInitError_None)
	{
		HandleVRError(err);
	}

	std::clog << GetTrackedDeviceString(m_hmd, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String) << std::endl;
	std::clog << GetTrackedDeviceString(m_hmd, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String) << std::endl;

}

std::string GameplayScreen::GetTrackedDeviceString(vr::IVRSystem * pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * peError)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
		if (unRequiredBufferLen == 0)
			return "";
	
		char *pchBuffer = new char[unRequiredBufferLen];
		unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
		std::string sResult = pchBuffer;
		delete[] pchBuffer;
		return sResult;
}


//void GameplayScreen::SubmitFramesOpenGL(GLint leftEyeTex, GLint rightEyeTex, bool linear = false)
//{
//	if (!m_hmd)
//	{
//		throw std::runtime_error("Error : presenting frames when VR system handle is NULL");
//	}
//
//	vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
//	vr::VRCompositor()->WaitGetPoses(trackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
//
//	vr::EColorSpace colorSpace = linear ? vr::ColorSpace_Linear : vr::ColorSpace_Gamma;
//
//	vr::Texture_t leftEyeTexture = { (void*)leftEyeTex, vr::TextureType_OpenGL, colorSpace };
//	vr::Texture_t rightEyeTexture = { (void*)rightEyeTex, vr::TextureType_OpenGL, colorSpace };
//
//	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
//	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
//
//	vr::VRCompositor()->PostPresentHandoff();
//}

