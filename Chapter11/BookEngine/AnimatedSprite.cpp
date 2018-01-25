#include "AnimatedSprite.h"

namespace BookEngine
{

	AnimatedSprite::AnimatedSprite(float frameTime, bool paused, bool looped) :
		m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0), m_isPaused(paused), m_isLooped(looped), m_texture(NULL)
	{

	}

	void AnimatedSprite::SetAnimation(const Animation& animation)
	{
		m_animation = &animation;
		m_texture = m_animation->GetSpriteSheet();
		m_currentFrame = 0;
	}

	void AnimatedSprite::SetFrameTime(float time)
	{
		m_frameTime = time;
	}

	void AnimatedSprite::Play()
	{
		m_isPaused = false;
	}

	void AnimatedSprite::Play(const Animation& animation)
	{
		if (GetAnimation() != &animation)
			SetAnimation(animation);
		Play();
	}

	void AnimatedSprite::Pause()
	{
		m_isPaused = true;
	}

	void AnimatedSprite::Stop()
	{
		m_isPaused = true;
		m_currentFrame = 0;
	}

	void AnimatedSprite::SetLooped(bool looped)
	{
		m_isLooped = looped;
	}

	const Animation* AnimatedSprite::GetAnimation() const
	{
		return m_animation;
	}

	glm::vec4 AnimatedSprite::GetLocalBounds() const
	{
		glm::vec4 rect = m_animation->GetFrame(m_currentFrame);

		float width = std::abs(rect.w);
		float height = std::abs(rect.z);

		return glm::vec4(0.f, 0.f, width, height);
	}

	bool AnimatedSprite::isLooped() const
	{
		return m_isLooped;
	}

	bool AnimatedSprite::isPlaying() const
	{
		return !m_isPaused;
	}

	float AnimatedSprite::GetFrameTime() const
	{
		return m_frameTime;
	}

	void AnimatedSprite::Update()
	{
		// if not paused and we have a valid animation
		if (!m_isPaused && m_animation)
		{
			// get next Frame index
			if (m_currentFrame + 1 < m_animation->GetSize())
				m_currentFrame++;
			else
			{
				// animation has ended
				m_currentFrame = 0; // reset to start
				if (!m_isLooped) m_isPaused = true;
			}
		}
	}

	void AnimatedSprite::Draw(SpriteBatch& spriteBatch, glm::vec2 renderPosition) const
	{
		if (m_animation && m_texture)
		{		
			spriteBatch.Draw(
				glm::vec4(renderPosition.x, renderPosition.y, GetLocalBounds().w, GetLocalBounds().z),
				glm::vec4(m_currentFrame*m_animation->GetFrame(m_currentFrame).x / m_texture->width,
					m_animation->GetFrame(m_currentFrame).y / m_texture->height,
					m_animation->GetFrame(m_currentFrame).w / m_texture->width,
					m_animation->GetFrame(m_currentFrame).z / m_texture->height),
				m_texture->id,
				0.0f,
				ColorRGBA8(255,255,255,255));
		}
	}
}
