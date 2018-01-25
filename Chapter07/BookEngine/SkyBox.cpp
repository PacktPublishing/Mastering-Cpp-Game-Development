#include "SkyBox.h"
#include "PNGdecoder.h"
#include "IOManager.h"
#include "Exception.h"
#include <stdlib.h>
#include <glm/gtc/type_ptr.hpp>

namespace BookEngine
{

	SkyBox::SkyBox()
	{
		//m_vertexbuffer = 0;
		//m_uvbuffer = 0;
		//m_normalsbuffer = 0;
		m_vao = 0;
	}


	SkyBox::~SkyBox()
	{

		if (m_vao != 0)
			glDeleteVertexArrays(1, &m_vao);
	}

	void SkyBox::Init(std::string front, std::string back, std::string top,
		std::string bottom, std::string left, std::string right, std::string vertShader, std::string fragShader)
	{
		float points[] = {
			-10.0f, 10.0f,	-10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	-10.0f, -10.0f,
			10.0f,	-10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, -10.0f, 10.0f,	-10.0f,

			-10.0f, -10.0f, 10.0f,	-10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	-10.0f,
			-10.0f, 10.0f,	-10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, -10.0f, 10.0f,

			10.0f,	-10.0f, -10.0f, 10.0f,	-10.0f, 10.0f,	10.0f,	10.0f,	10.0f,
			10.0f,	10.0f,	10.0f,	10.0f,	10.0f,	-10.0f, 10.0f,	-10.0f, -10.0f,

			-10.0f, -10.0f, 10.0f,	-10.0f, 10.0f,	10.0f,	10.0f,	10.0f,	10.0f,
			10.0f,	10.0f,	10.0f,	10.0f,	-10.0f, 10.0f,	-10.0f, -10.0f, 10.0f,

			-10.0f, 10.0f,	-10.0f, 10.0f,	10.0f,	-10.0f, 10.0f,	10.0f,	10.0f,
			10.0f,	10.0f,	10.0f,	-10.0f, 10.0f,	10.0f,	-10.0f, 10.0f,	-10.0f,

			-10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, -10.0f,
			10.0f,	-10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, 10.0f
		};
		

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &points,
			GL_STATIC_DRAW);

		//Generate the VAO if it isn't already generated
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
		}

		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


		// generate a cube-map texture to hold all the sides
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &(m_texture.id));

		

		// load each image and copy into a side of the cube-map texture
		(LoadSide(m_texture.id, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
		(LoadSide(m_texture.id, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
		(LoadSide(m_texture.id, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
		(LoadSide(m_texture.id, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
		(LoadSide(m_texture.id, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
		(LoadSide(m_texture.id, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
		// format cube map texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Create and compile our GLSL program from the shaders
		m_shaderManager.CompileShaders(vertShader, fragShader);
		m_shaderManager.AddAttribute("vp");
		m_shaderManager.LinkShaders();

		// Get a handle for our "MVP" a.k.a ModelViewProjection uniform
		m_viewMatrixID = m_shaderManager.GetUniformLocation("View");
		m_projMatrixID = m_shaderManager.GetUniformLocation("Projection");

		


		//Unbind the VBO
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	
	void SkyBox::LoadSide(GLuint& texture, GLenum side_target, std::string filePath) {
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture.id);

		//This is the input data to decodePNG, which we load from a file
		std::vector<unsigned char> in;
		//This is the output data from decodePNG, which is the pixel data for our texture
		std::vector<unsigned char> out;

		unsigned long width, height;

		//Read in the image file contents into a buffer
		if (IOManager::ReadFileToBuffer(filePath, in) == false) {
			Exception("Failed to load PNG file to buffer!");
		}

		//Decode the .png format into an array of pixels
		int errorCode = DecodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0) {
			Exception("decodePNG failed with error: " + std::to_string(errorCode));
		}

	
		// copy image data into 'target' side of cube map
		glTexImage2D(side_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			&(out[0]));

	}


	
	void SkyBox::Draw(Camera3D * camera)
	{
		

		//glDepthMask(GL_FALSE);

		m_shaderManager.Use();
		
		// Get a handle for our "MVP" a.k.a ModelViewProjection uniform
		m_viewMatrixID = m_shaderManager.GetUniformLocation("View");
		m_projMatrixID = m_shaderManager.GetUniformLocation("Projection");
		
		//glUseProgram(cube_sp);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		//glm::mat4 test = glm::inverse(camera.GetView());
		glUniformMatrix4fv(m_projMatrixID, 1, GL_FALSE, glm::value_ptr(camera->GetProjection()));
		glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, glm::value_ptr(camera->GetView()));
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture.id);
		
		GLint textureUniform = m_shaderManager.GetUniformLocation("TextureSampler");
		glUniform1i(textureUniform, 0);
		
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		m_shaderManager.UnUse();

		glDisableVertexAttribArray(0);
		
	}
}