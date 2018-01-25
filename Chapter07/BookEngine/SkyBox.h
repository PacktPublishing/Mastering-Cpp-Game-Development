#pragma once
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
// Include GLEW
#include <GL/glew.h>
#include "GLTexture.h"
// Include GLM
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "ShaderManager.h"
#include "Camera3D.h"

namespace BookEngine
{
	class SkyBox
	{
	public:
		SkyBox();
		~SkyBox();

		void SkyBox::Init(std::string front, std::string back, std::string top,
			std::string bottom, std::string left, std::string right, std::string vertShader, std::string fragShader);

		void Draw(Camera3D* camera);
	private:
		GLuint m_vao;
		GLuint m_vertexbuffer;
		GLuint m_uvbuffer;
		GLuint m_normalsbuffer;
		GLTexture m_texture;

		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec2> m_uvs;
		std::vector<glm::vec3> m_normals;

		BookEngine::ShaderManager m_shaderManager;
		GLuint m_projMatrixID;
		GLuint m_viewMatrixID;

		void LoadSide(GLuint& texture, GLenum side_target, std::string filePath);

	};
}

