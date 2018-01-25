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
#include "ParticleManager2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace BookEngine
{

	ParticleManager2D::ParticleManager2D()
	{
	}


	ParticleManager2D::~ParticleManager2D()
	{
		for (auto& batch : m_batches)
			delete batch;
	}

	void ParticleManager2D::CreateParticleBatch(ParticleBatch2D * particleBatch)
	{
		m_batches.push_back(particleBatch);
	}

	void ParticleManager2D::Update(float deltaTime)
	{
		for (auto& batch : m_batches)
			batch->Update(deltaTime);
	}

	void ParticleManager2D::Draw(SpriteBatch* spriteBatch)
	{
		for (auto& batch : m_batches)
		{
			spriteBatch->Begin();
			batch->Draw(spriteBatch);
			spriteBatch->End();
			spriteBatch->BatchRender();
		}
	}
	
}
