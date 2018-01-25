/*
    Copyright (c) 2016 Michael "Mickey" MacDonald.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Vertex2d.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace BookEngine {

    class Particle2D {
    public:
        glm::vec2 position = glm::vec2(0.0f);
        glm::vec2 velocity = glm::vec2(0.0f);
        ColorRGBA8 color;
        float life = 0.0f;
        float width = 0.0f;
    };

    // Default function pointer
    inline void DefaultParticleUpdate(Particle2D& particle, float deltaTime) {
        particle.position += particle.velocity * deltaTime;
    }

    class ParticleBatch2D {
    public:
        ParticleBatch2D();
        ~ParticleBatch2D();

        void Init(int maxParticles,
                  float decayRate,
                  GLTexture texture,
                  std::function<void(Particle2D&, float)> updateFunc = DefaultParticleUpdate);

        void Update(float deltaTime);

        void Draw(SpriteBatch* spriteBatch);

        void AddParticle(const glm::vec2& position,
                         const glm::vec2& velocity,
                         const ColorRGBA8& color,
                         float width);

    private:
        int FindFreeParticle();

        std::function<void(Particle2D&, float)> m_updateFunc; ///< Function pointer for custom updates
        float m_decayRate = 0.1f;
        Particle2D* m_particles = nullptr;
        int m_maxParticles = 0;
        int m_lastFreeParticle = 0;
        GLTexture m_texture;
    };

}