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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BookEngine {

    //Camera class for 2D games
    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        //sets up the orthographic matrix and screen dimensions
        void Init(int screenWidth, int screenHeight);

        //updates the camera matrix if needed
        void Update();

        glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);

        bool InView(const glm::vec2& position, const glm::vec2& dimensions);

        //setters
        void SetPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true; }
        void SetScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

        //getters
        glm::vec2 GetPosition() { return m_position; }
        float GetScale() { return m_scale; }
        glm::mat4 GetCameraMatrix() { return m_cameraMatrix; }

    private:
        int m_screenWidth, m_screenHeight;
        bool m_needsMatrixUpdate;
        float m_scale;
        glm::vec2 m_position;
        glm::mat4 m_cameraMatrix;
        glm::mat4 m_orthoMatrix;
    };

}