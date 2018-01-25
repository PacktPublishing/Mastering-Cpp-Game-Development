#pragma once
#include "GLTexture.h"
#include <glm/glm.hpp>
#include <vector>
#include "SpriteBatch.h"

namespace BookEngine
{

	class Animation
	{
	public:
		Animation();

		void AddFrame(glm::vec4 cellRectangle);
		void SetSpriteSheet(const GLTexture& texture);
		const GLTexture* GetSpriteSheet() const;
		std::size_t GetSize() const;
		const glm::vec4 GetFrame(std::size_t n) const;

	private:
		std::vector<glm::vec4> m_frames;
		const GLTexture* m_texture;
	};

	/*class Animation
	{
	public:
		Animation();
		~Animation();
		
		void Update();

		void SetSpriteSheet(GLTexture& spriteSheet) { m_texture = spriteSheet; };
		void AddFrame(glm::vec4 cellRectangle);
		void Draw(SpriteBatch& spriteBatch, glm::vec2 position);
	private:
		GLTexture m_texture;
		std::vector<glm::vec4> m_animationCells;
		unsigned m_currentFrame;
		unsigned m_totalFrames;
		const glm::vec4 m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	};*/
}

