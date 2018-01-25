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
#include "Mesh.h"
#include "MeshLoader.h"
#include "ResourceManager.h"

namespace BookEngine
{

	Mesh::Mesh()
	{
		m_vertexbuffer = 0;
		m_uvbuffer = 0;
		m_vao = 0;
	}


	Mesh::~Mesh()
	{
		//Always remember to delete your buffers when
		//you are done!
		if (m_vertexbuffer != 0) 
			glDeleteBuffers(1, &m_vertexbuffer);
		
		if (m_uvbuffer != 0) 
			glDeleteBuffers(1, &m_uvbuffer);
	
		if (m_vao != 0)
			glDeleteVertexArrays(1, &m_vao);
	}
	void Mesh::Init(std::string pathToMesh, std::string pathToTexture)
	{	
		
		// Read our .obj file
		bool res = LoadOBJ(pathToMesh, m_vertices, m_uvs, m_normals);
		
		// Load Texture
		m_texture = ResourceManager::GetTexture(pathToTexture);
        
		//Generate the VAO if it isn't already generated
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
		}

		//Bind the VAO. All subsequent opengl calls will modify it's state.
		glBindVertexArray(m_vao);


		//Generate the buffers if they havent already been generated
		if (m_vertexbuffer == 0) {
			glGenBuffers(1, &m_vertexbuffer);
		}
		if (m_uvbuffer == 0) {
			glGenBuffers(1, &m_uvbuffer);
		}

		// Load it into a VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
	}

	void Mesh::Draw()
	{	
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture.id);

		// 1st attribute buffer : m_vertices
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : m_uvbuffer
		glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles!
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		
		
	}
}
