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
#include "ParticleBatch2D.h"

namespace BookEngine {

    ParticleBatch2D::ParticleBatch2D() {
        // Empty
    }


    ParticleBatch2D::~ParticleBatch2D() {
        delete[] m_particles;
    }

    void ParticleBatch2D::Init(int maxParticles,
                               float decayRate,
                               GLTexture texture,
                               std::function<void(Particle2D&, float)> updateFunc /* = defaultParticleUpdate */) {
        m_maxParticles = maxParticles;
        m_particles = new Particle2D[maxParticles];
        m_decayRate = decayRate;
        m_texture = texture;
        m_updateFunc = updateFunc;
    }

    void ParticleBatch2D::Update(float deltaTime) {
        for (int i = 0; i < m_maxParticles; i++) {
            // Check if it is active
            if (m_particles[i].life > 0.0f) {
                // Update using function pointer
                m_updateFunc(m_particles[i], deltaTime);
                m_particles[i].life -= m_decayRate * deltaTime;
            }
        }
    }

    void ParticleBatch2D::Draw(SpriteBatch* spriteBatch) {
        glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        for (int i = 0; i < m_maxParticles; i++) {
            // Check if it is active
            auto& p = m_particles[i];
            if (p.life > 0.0f) {
                glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
                spriteBatch->Draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
            }
        }
    }

    void ParticleBatch2D::AddParticle(const glm::vec2& position,
                                      const glm::vec2& velocity,
                                      const ColorRGBA8& color,
                                      float width) {
        int particleIndex = FindFreeParticle();

        auto& p = m_particles[particleIndex];

        p.life = 1.0f;
        p.position = position;
        p.velocity = velocity;
        p.color = color;
        p.width = width;
    }

    int ParticleBatch2D::FindFreeParticle() {

        for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
            if (m_particles[i].life <= 0.0f) {
                m_lastFreeParticle = i;
                return i;
            }
        }

        for (int i = 0; i < m_lastFreeParticle; i++) {
            if (m_particles[i].life <= 0.0f) {
                m_lastFreeParticle = i;
                return i;
            }
        }

        // No particles are free, overwrite first particle
        return 0;
    }

}