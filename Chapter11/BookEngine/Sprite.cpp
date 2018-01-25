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
#include "Sprite.h"
#include "Vertex2D.h"
#include "ResourceManager.h"

#include <cstddef>

namespace BookEngine {

    Sprite::Sprite()
    {
        _vboID = 0;
    }


    Sprite::~Sprite()
    {
        //Always remember to delete your buffers when
        //you are done!
        if (_vboID != 0) {
            glDeleteBuffers(1, &_vboID);
        }
    }

    //Initializes the sprite VBO. x, y, width, and height are
    //in the normalized device coordinate space. so, [-1, 1]
    void Sprite::Init(float x, float y, float width, float height, std::string texturePath) {
        //Set up our private vars
        _x = x;
        _y = y;
        _width = width;
        _height = height;

        _texture = ResourceManager::GetTexture(texturePath);
        
        // Generate the VAO if it isn't already generated
        if (m_vao == 0) {
            glGenVertexArrays(1, &m_vao);
        }
        
        // Bind the VAO. All subsequent opengl calls will modify it's state.
        glBindVertexArray(m_vao);

        //Generate the buffer if it hasn't already been generated
        if (_vboID == 0) {
            glGenBuffers(1, &_vboID);
        }

        //This array will hold our vertex data.
        //We need 6 vertices, and each vertex has 2
        //floats for X and Y
        Vertex2D vertexData[6];

        //First Triangle
        vertexData[0].SetPosition(x + width, y + height);
        vertexData[0].SetUV(1.0f, 1.0f);

        vertexData[1].SetPosition(x, y + height);
        vertexData[1].SetUV(0.0f, 1.0f);

        vertexData[2].SetPosition(x, y);
        vertexData[2].SetUV(0.0f, 0.0f);

        //Second Triangle
        vertexData[3].SetPosition(x, y);
        vertexData[3].SetUV(0.0f, 0.0f);

        vertexData[4].SetPosition(x + width, y);
        vertexData[4].SetUV(1.0f, 0.0f);

        vertexData[5].SetPosition(x + width, y + height);
        vertexData[5].SetUV(1.0f, 1.0f);

        //Set all vertex colors
        for (int i = 0; i < 6; i++) {
            vertexData[i].SetColor(255, 255, 255, 255);
        }

        //Tell opengl to bind our vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, _vboID);
        //Upload the data to the GPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
        //Unbind the buffer (optional)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //Draws the sprite to the screen
    void Sprite::Draw() {

        glBindTexture(GL_TEXTURE_2D, _texture.id);

		// Generate the VAO if it isn't already generated
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
		}

		// Bind the VAO. All subsequent opengl calls will modify it's state.
		glBindVertexArray(m_vao);

        //bind the buffer object
        glBindBuffer(GL_ARRAY_BUFFER, _vboID);

        //Tell opengl what attribute arrays we need
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

        //Draw the 6 vertices to the screen
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Disable the vertex attrib arrays. This is not optional.
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        //Unbind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}