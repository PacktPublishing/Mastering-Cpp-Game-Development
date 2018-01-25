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
#include "ShaderManager.h"
#include "Exception.h"

#include <vector>

#include <fstream>

namespace BookEngine {

    //The : _numAttributes(0) ect. is an Initialization list. It is a better way to Initialize variables, since it avoids an extra copy. 
    ShaderManager::ShaderManager() : m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
    {

    }


    ShaderManager::~ShaderManager()
    {
    }

    //Compiles the shaders into a form that your GPU can understand
    void ShaderManager::CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath) {
        //Vertex and fragment shaders are successfully compiled.
        //Now time to link them together into a program.
        //Get a program object.
        m_programID = glCreateProgram();

        //Create the vertex shader object, and store its ID
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if (m_vertexShaderID == 0) {
            throw Exception("Vertex shader failed to be created!");
        }

        //Create the fragment shader object, and store its ID
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if (m_fragmentShaderID == 0) {
			throw Exception("Fragment shader failed to be created!");
        }

        //Compile each shader
        CompileShader(vertexShaderFilePath, m_vertexShaderID);
        CompileShader(fragmentShaderFilepath, m_fragmentShaderID);
    }

    void ShaderManager::LinkShaders() {

        //Attach our shaders to our program
        glAttachShader(m_programID, m_vertexShaderID);
        glAttachShader(m_programID, m_fragmentShaderID);

        //Link our program
        glLinkProgram(m_programID);

        //Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);



            //We don't need the program anymore.
            glDeleteProgram(m_programID);
            //Don't leak shaders either.
            glDeleteShader(m_vertexShaderID);
            glDeleteShader(m_fragmentShaderID);

            //print the error log and quit
            std::printf("%s\n", &(errorLog[0]));
			throw Exception("Shaders failed to link!");
        }

        //Always detach shaders after a successful link.
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
    }

    //Adds an attribute to our shader. SHould be called between compiling and linking.
    void ShaderManager::AddAttribute(const std::string& attributeName) {
        glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
    }

    GLint ShaderManager::GetUniformLocation(const std::string& uniformName) {
        GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
        if (location == GL_INVALID_INDEX) {
			throw Exception("Uniform " + uniformName + " not found in shader!");
        }
        return location;
    }

    //enable the shader, and all its attributes
    void ShaderManager::Use() {
        glUseProgram(m_programID);
        //enable all the attributes we added with addAttribute
        for (int i = 0; i < m_numAttributes; i++) {
            glEnableVertexAttribArray(i);
        }
    }

    //disable the shader
    void ShaderManager::UnUse() {
        glUseProgram(0);
        for (int i = 0; i < m_numAttributes; i++) {
            glDisableVertexAttribArray(i);
        }
    }

    //Compiles a single shader file
    void ShaderManager::CompileShader(const std::string& filePath, GLuint id) {

        //Open the file
        std::ifstream shaderFile(filePath);
        if (shaderFile.fail()) {
            perror(filePath.c_str());
			throw Exception("Failed to open " + filePath);
        }

        //File contents stores all the text in the file
        std::string fileContents = "";
        //line is used to grab each line of the file
        std::string line;

        //Get all the lines in the file and add it to the contents
        while (std::getline(shaderFile, line)) {
            fileContents += line + "\n";
        }

        shaderFile.close();

        //get a pointer to our file contents c string;
        const char* contentsPtr = fileContents.c_str();
        //tell opengl that we want to use fileContents as the contents of the shader file
        glShaderSource(id, 1, &contentsPtr, nullptr);

        //compile the shader
        glCompileShader(id);

        //check for errors
        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

            //Provide the infolog in whatever manor you deem best.
            //Exit with failure.
            glDeleteShader(id); //Don't leak the shader.

            //Print error log and quit
            std::printf("%s\n", &(errorLog[0]));
			throw Exception("Shader " + filePath + " failed to compile");
        }
    }

}