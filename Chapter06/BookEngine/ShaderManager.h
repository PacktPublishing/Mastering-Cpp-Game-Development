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

#include <string>
#include <GL/glew.h>

namespace BookEngine {

    //This class handles the compilation, linking, and usage of a GLSL shader program.
    //Reference: http://www.opengl.org/wiki/Shader_Compilation
    class ShaderManager
    {
    public:
        ShaderManager();
        ~ShaderManager();

        void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath);

        void LinkShaders();

        void AddAttribute(const std::string& attributeName);

        GLint GetUniformLocation(const std::string& uniformName);

        void Use();
        void UnUse();
    private:

        int m_numAttributes;

        void CompileShader(const std::string& filePath, GLuint id);

        GLuint m_programID;

        GLuint m_vertexShaderID;
        GLuint m_fragmentShaderID;
    };

}
