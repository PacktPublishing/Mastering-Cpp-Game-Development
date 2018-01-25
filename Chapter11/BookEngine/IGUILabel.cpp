#include "IGUILabel.h"

namespace BookEngine
{



	IGUILabel::IGUILabel(glm::vec4 & bounds, glm::vec2 & position, std::string text, SpriteFont * font, glm::vec2 & fontScale, IGUIPanel * panel) :
		m_labelText(text),
		m_spriteFont(font),
		m_fontScale(fontScale),
		m_panel(panel)
	{
		m_bounds = bounds;
		
		if (m_panel != NULL)
		{
			m_position = *m_panel->GetPosition() + position;

			//Move to just below the last element in the list 
			if (!m_panel->GetGUIObjectList().empty())
			{
				IGUIObject* lastButton = m_panel->GetGUIObjectList().back();
				m_position.y = lastButton->GetPosition().y -
					lastButton->GetBounds().w -
					2.0f; // Used as default padding (should be dynamic)
			}
			else
			{
				//Move to top of pannel
				m_position.y += m_panel->GetBounds()->w - m_bounds.w;
			}
		}
	}

	IGUILabel::~IGUILabel()
	{
	}
	void IGUILabel::Update()
	{
	}
	void IGUILabel::Draw(SpriteBatch & spriteBatch)
	{
		glDepthMask(GL_FALSE);
		char buffer[256];
		sprintf_s(buffer, m_labelText.c_str());
		m_spriteFont->Draw(spriteBatch, buffer, glm::vec2(m_position.x + (m_bounds.z * 0.5f), (m_position.y + (m_bounds.w * 0.5f)) - ((m_spriteFont->GetFontHeight() * m_fontScale.y) * 0.5f)),
			m_fontScale, 0.2f, BookEngine::ColorRGBA8(255, 255, 255, 255), Justification::MIDDLE);
	}
	void IGUILabel::OnNotify(IGUIObject & button, GUIEvent event)
	{
		if (event == GUIEvent::Clicked)
		{
			OnClick();
		}

	}

	void IGUILabel::OnClick()
	{
		m_clicked = true;
	}
	
}
