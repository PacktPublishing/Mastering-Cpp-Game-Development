#include "NPC.h"
#include <iostream>



NPC::NPC(DialogGUI& gui) : m_gui(&gui)
{

	std::cout << "Loading Scripts \n";
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	lua.script_file("Scripts/test.lua");

	currentState = InteractionState::Display;

}


NPC::~NPC()
{
}

void NPC::Say(std::string stringToSay)
{
	//std::cout << stringToSay << std::endl;
	m_gui->SetDialog(stringToSay);
}


void NPC::PresentOptions()
{
	
	sol::table choices = lua["CurrentDialog"]["choices"];

	int i = 0;
	if (choices.valid())
	{
		choices.for_each([&](sol::object const& key, sol::object const& value)
		{
			m_gui->SetOption(value.as<std::string>(), i);
			i++;
		});
	}
}

void NPC::Update(float deltaTime)
{
	
	switch (currentState)
	{
	case InteractionState::Display:
		
		
		Say(lua["CurrentDialog"]["say"]);
		PresentOptions();
		currentState = InteractionState::WaitingForInput;
		break;
	case InteractionState::WaitingForInput:
		for (int i = 0; i < m_gui->choices.size(); i++)
		{
			if (m_gui->choices[i]->GetClickedStatus() == true)
			{
				//return i;
				std::cout << "TEST";
				lua["CurrentDialog"]["onSelection"](m_gui->choices[i]->GetLabelText());
				currentState = InteractionState::Display;
				m_gui->choices.clear();
				m_gui->RemoveAllPanelObjects();
				
				
			}
		}
		
		break;
	}

	
}
