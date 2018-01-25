#include "IGUIButton.h"


namespace BookEngine {

	IGUIButton::IGUIButton(glm::vec4& bounds, glm::vec2& position, std::string label, std::vector<GLTexture*> textures, SpriteFont* font, glm::vec2& fontScale, IGUIPanel* panel) :
		
		m_textures(textures),
		m_buttonLabel(label),
		m_spriteFont(font),
		m_fontScale(fontScale),
		m_panel(panel)
	{
		m_bounds = bounds;
		//m_currentButtonStatus = GUIButtonStatus::Default;
		if (m_panel != NULL)
		{
			m_position = *m_panel->GetPosition() + position;
			
			//Move to just below the last element in the list 
			if (!m_panel->GetGUIObjectList().empty())
			{
				IGUIObject* lastButton = m_panel->GetGUIObjectList().back();
				m_position.y = lastButton->GetPosition().y +
							   lastButton->GetBounds().w +
							   10.0f; // Used as default padding (should be dynamic)
			}
			else
			{
				//Move to top of pannel
				m_position.y += m_panel->GetBounds()->w - m_bounds.w;
			}
		}
	}

	IGUIButton::IGUIButton(glm::vec4 & bounds, glm::vec2 & position, std::string label, GLTexture * texture, SpriteFont* font, glm::vec2& fontScale, IGUIPanel* panel) :
		m_texture(*texture),
		m_buttonLabel(label),
		m_spriteFont(font),
		m_fontScale(fontScale),
		m_panel(panel)
	{

		m_bounds = bounds;
		if (m_panel != NULL)
		{
			//m_currentButtonStatus = GUIButtonStatus::Default;
			m_position = *m_panel->GetPosition() + position;
			//Move to top of pannel
			if (!m_panel->GetGUIObjectList().empty())
			{
				IGUIObject* lastButton = m_panel->GetGUIObjectList().back();
				m_position.y = lastButton->GetPosition().y -
					lastButton->GetBounds().w -
					10.0f;
			}
			else
			{
				m_position.y += m_panel->GetBounds()->w - m_bounds.w;
			}
		}
	}

	IGUIButton::~IGUIButton()
	{
	}

	void IGUIButton::Update()
	{
		/*switch (m_currentButtonStatus)
		{
		case GUIButtonStatus::Clicked:
			OnClick();
			break;
		case GUIButtonStatus::Released:
			OnRelease();
			break;
		case GUIButtonStatus::Default:
			break;
		}*/
	}

	void IGUIButton::Draw(SpriteBatch& spriteBatch)
	{
		glDepthMask(GL_FALSE);
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		spriteBatch.Draw(glm::vec4(m_position.x, m_position.y, m_bounds.z, m_bounds.w), uvRect, m_texture.id, 0.0f, ColorRGBA8(255, 255, 255, 255));

		char buffer[256];
		sprintf_s(buffer, m_buttonLabel.c_str());
		m_spriteFont->Draw(spriteBatch, buffer, glm::vec2(m_position.x + (m_bounds.z * 0.5f), (m_position.y + (m_bounds.w * 0.5f)) - ((m_spriteFont->getFontHeight() * m_fontScale.y) * 0.5f)),
			m_fontScale, 0.2f, BookEngine::ColorRGBA8(255, 255, 255, 255), Justification::MIDDLE);
		//glDepthMask(GL_TRUE);
		/*switch (m_currentButtonStatus)
		{
		case BookEngine::GUIButtonStatus::Released:
		spriteBatch.Draw(m_bounds, uvRect, m_textures[0]->id, 0.0f, BookEngine::ColorRGBA8(255, 255, 255, 255));
		break;
		case BookEngine::GUIButtonStatus::Clicked:
		spriteBatch.Draw(m_bounds, uvRect, m_textures[1]->id, 0.0f, BookEngine::ColorRGBA8(255, 255, 255, 255));
		break;
		}*/
	}
	void IGUIButton::OnClick()
	{
		/*m_currentButtonStatus = GUIButtonStatus::Released;*/
	}
	void IGUIButton::OnRelease()
	{
		/*m_currentButtonStatus = GUIButtonStatus::Default;*/
	}

	void IGUIButton::OnNotify(IGUIObject & button, GUIEvent event)
	{
		switch (event)
		{
		case(GUIEvent::HoverOver):
			//Handle Hover
			break;
		case(GUIEvent::Clicked):
			OnClick();
			break;
		}

	}
	

}