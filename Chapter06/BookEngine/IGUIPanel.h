#pragma once
#include "IGUIObject.h"
#include "InputManager.h"
#include <glm/glm.hpp>

namespace BookEngine
{
	static enum class GUIAnchorPos {
		TopRight,
		TopLeft,
		BottomRight,
		BottomLeft,
		Center
	};

	class IGUIPanel
	{
	public:
		IGUIPanel(glm::vec4 panelBounds = glm::vec4(0,0,200,480), 
			glm::vec2 panelAnchor = glm::vec2(0,0),
			glm::vec2 offset = glm::vec2(0,0));
		virtual ~IGUIPanel();	
		virtual void AddGUIObject(IGUIObject& GUIObject);
		virtual void Update();
		virtual void Draw(SpriteBatch& spriteBatch);
		virtual glm::vec2* GetPosition() { return &m_Pos; };
		virtual glm::vec4* GetBounds() { return &m_bounds; };
		virtual void CheckInput(InputManager& inputManager);
		virtual std::vector<IGUIObject*>& GetGUIObjectList() { return m_GUIObjectsList; };


	protected:
		std::vector<IGUIObject*> m_GUIObjectsList;
		glm::vec4 m_bounds;
		glm::vec2 m_offset;
		glm::vec2 m_Pos;
		float m_panelWidth, m_panelHeight;
	};
}

