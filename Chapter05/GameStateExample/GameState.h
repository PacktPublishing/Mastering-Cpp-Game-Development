#pragma once
#include <BookEngine\IState.h>

class GameState : BookEngine::IState
{
public:
	GameState();
	~GameState();

	//Our overrides
	virtual void OnEntry() = 0;
	virtual void OnExit() = 0;
	virtual void Update(float deltaTime) = 0;

	//Added specialty function
	virtual void Draw() = 0;
};

