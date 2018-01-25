#pragma once
#include "IGUIObject.h"
#include "IGUIPanel.h"
#include "SpriteFont.h"
#include <glm/glm.hpp>
#include "GLTexture.h"
#include <vector>
#include <string>

namespace BookEngine {

	class IGUIButton :
		public IGUIObject
	{
	public:
		IGUIButton(glm::vec4& bounds, glm::vec2& position, std::string label, std::vector<GLTexture*> textures, SpriteFont* font, glm::vec2& fontScale = glm::vec2(1.0f), IGUIPanel* panel = NULL);
		IGUIButton(glm::vec4& bounds, glm::vec2& position, std::string label, GLTexture* texture, SpriteFont* font, glm::vec2& fontScale = glm::vec2(1.0f), IGUIPanel* panel = NULL);
		~IGUIButton();

		virtual void Update() override;
		virtual void Draw(SpriteBatch& spriteBatch) override;

		virtual void OnClick();
		virtual void OnRelease();
		virtual void OnNotify(IGUIObject& button, GUIEvent event) override;

	protected:
		std::vector<GLTexture*> m_textures;
		GLTexture m_texture;
		std::string m_buttonLabel;
		SpriteFont* m_spriteFont;
		IGUIPanel* m_panel;
		glm::vec2 m_fontScale;
	};

}