#pragma once
#include <string>
#include <vector>
#include <sol/sol.hpp>
#include "DialogGUI.h"
class NPC
{
	enum InteractionState
	{
		Display,
		WaitingForInput,
	};

public:
	NPC(DialogGUI& gui);
	~NPC();

	void Say(std::string stringToSay);
	void PresentOptions();
	void Update(float deltaTime);
	

private:
	std::vector<std::string> m_options;
	
	sol::state lua;
	DialogGUI* m_gui;
	InteractionState currentState;
};
