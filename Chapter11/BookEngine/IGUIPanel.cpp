#include "IGUIPanel.h"
#include <algorithm>
#include <memory>
#include "ResourceManager.h"


namespace BookEngine
{

	IGUIPanel::IGUIPanel(glm::vec4 panelBounds, glm::vec2 panelAnchor, glm::vec2 offset) : m_bounds(panelBounds), m_offset(offset)
	{
		m_Pos = panelAnchor + m_offset;
		m_panelWidth = m_bounds.z;
		m_panelHeight = m_bounds.w;
	}

	IGUIPanel::~IGUIPanel()
	{
		std::for_each(m_GUIObjectsList.begin(), m_GUIObjectsList.end(), std::default_delete<IGUIObject>());
	}

	void BookEngine::IGUIPanel::AddGUIObject(IGUIObject & GUIObject)
	{
		m_GUIObjectsList.push_back(&GUIObject);
	}
	void IGUIPanel::Draw(SpriteBatch& spriteBatch)
	{
		spriteBatch.Draw(glm::vec4(m_Pos.x, m_Pos.y, m_panelWidth, m_panelHeight),glm::vec4(0,0,1,1), BookEngine::ResourceManager::GetTexture("Textures/floor.png").id, -0.1f, ColorRGBA8(0,0,0,75));

		for (auto const& value : m_GUIObjectsList)
		{
			value->Draw(spriteBatch);
		}
	}

	void IGUIPanel::CheckInput(InputManager& inputManager)
	{

	}
		
	
	void IGUIPanel::Update()
	{
		for (auto const& value : m_GUIObjectsList)
		{
			value->Update();
		}
	}
}