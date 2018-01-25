#pragma once
#include <BookEngine/IState.h>

class GameWaiting : BookEngine::IState
{
	void OnEnter();
	void OnExit();
	void Update();
};

class GameRunning : BookEngine::IState
{
	void OnEnter();
	void OnExit();
	void Update();
};

class GameOver : BookEngine::IState
{
	void OnEnter();
	void OnExit();
	void Update();
};

