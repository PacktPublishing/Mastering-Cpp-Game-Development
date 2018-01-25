#include "Animation.h"

namespace BookEngine
{

	Animation::Animation() : m_texture(NULL)
	{

	}

	void Animation::AddFrame(glm::vec4 cellRectangle)
	{
		m_frames.push_back(cellRectangle);
	}

	void Animation::SetSpriteSheet(const GLTexture& texture)
	{
		m_texture = &texture;
	}

	const GLTexture* Animation::GetSpriteSheet() const
	{
		return m_texture;
	}

	std::size_t Animation::GetSize() const
	{
		return m_frames.size();
	}

	const glm::vec4 Animation::GetFrame(std::size_t n) const
	{
		return m_frames[n];
	}

	/*Animation::Animation() :
		m_totalFrames(0)
	{

	}


	Animation::~Animation()
	{
	}

	void Animation::Update() 
	{
		m_currentFrame++;
		if (m_currentFrame >= m_totalFrames)
			m_currentFrame = 0;
	}

	void Animation::AddFrame(glm::vec4 cellRectangle)
	{
		m_animationCells.emplace_back(cellRectangle);
		m_totalFrames++;
	}

	void Animation::Draw(SpriteBatch & spriteBatch, glm::vec2 position)
	{
		for each (glm::vec4 animation in m_animationCells)
		{
			glm::vec4 destinationRectangle = glm::vec4((int)position.x, (int)position.y, animation.w, animation.z);
			glm::vec4 uvRect((animation.x / (float)(animation.w)), animation.y / (float)(animation.z), (float)animation.w / (float)m_texture.width, (float)animation.z / (float)m_texture.height);
			spriteBatch.Draw(destinationRectangle, uvRect, m_texture.id, 0.0f, BookEngine::ColorRGBA8(255, 255, 255, 255));
		}

	}*/
}
