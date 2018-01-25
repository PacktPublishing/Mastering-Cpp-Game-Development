#pragma once
#include "GLTexture.h"
#include "SpriteBatch.h"
#include <glm/glm.hpp>
#include "Animation.h"

namespace BookEngine
{


	class AnimatedSprite 
	{
	public:
		explicit AnimatedSprite(float frameTime = 0.2f, bool paused = false, bool looped = true);

		//void Update(float deltaTime);
		void Update();
		void SetAnimation(const Animation& animation);
		void SetFrameTime(float time);
		void Play();
		void Play(const Animation& animation);
		void Pause();
		void Stop();
		void SetLooped(bool looped);
		const Animation* GetAnimation() const;
		glm::vec4 GetLocalBounds() const;
		bool isLooped() const;
		bool isPlaying() const;
		float GetFrameTime() const;

		void Draw(SpriteBatch& spriteBatch, glm::vec2 renderPosition) const;

	private:
		const Animation* m_animation;
		float m_frameTime;
		float m_currentTime;
		std::size_t m_currentFrame;
		bool m_isPaused;
		bool m_isLooped;
		const GLTexture* m_texture;
		Vertex2D m_vertices[4];
	};
}

