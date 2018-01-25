#pragma once
#include "GameState.h"

class GameWaiting : GameState
{
	virtual void OnEntry() override;
	virtual void OnExit() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};

class GameRunning : GameState
{
	virtual void OnEntry() override;
	virtual void OnExit() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};

class GameOver : GameState
{
	virtual void OnEntry() override;
	virtual void OnExit() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};

