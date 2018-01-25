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
#include "Camera2D.h"

namespace BookEngine {

    Camera2D::Camera2D() : m_position(0.0f, 0.0f),
        m_cameraMatrix(1.0f),
        m_orthoMatrix(1.0f),
        m_scale(1.0f),
        m_needsMatrixUpdate(true),
        m_screenWidth(500),
        m_screenHeight(500)
    {
    }


    Camera2D::~Camera2D()
    {
    }

    void Camera2D::Init(int screenWidth, int screenHeight) {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;
        m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
    }

    //updates the camera matrix if needed
    void Camera2D::Update() {

        //Only update if our position or scale have changed
        if (m_needsMatrixUpdate) {
            
            //Camera Translation
            glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
            m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

            //Camera Scale
            glm::vec3 scale(m_scale, m_scale, 0.0f);
            m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

            m_needsMatrixUpdate = false;
        }
    }

    glm::vec2 Camera2D::ConvertScreenToWorld(glm::vec2 screenCoords) {
        // Invert Y direction
        screenCoords.y = m_screenHeight - screenCoords.y;
        // Make it so that 0 is the center
        screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
        // Scale the coordinates
        screenCoords /= m_scale;
        // Translate with the camera position
        screenCoords += m_position;
        return screenCoords;
    }

    // Simple AABB test to see if a box is in the camera view
    bool Camera2D::InView(const glm::vec2& position, const glm::vec2& dimensions) {

        glm::vec2 scaledScreenDimensions = glm::vec2(m_screenWidth, m_screenHeight) / (m_scale);

        // The minimum distance before a collision occurs
        const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
        const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

        // Center position of the parameters
        glm::vec2 centerPos = position + dimensions / 2.0f;
        // Center position of the camera
        glm::vec2 centerCameraPos = m_position;
        // Vector from the input to the camera
        glm::vec2 distVec = centerPos - centerCameraPos;

        // Get the depth of the collision
        float xDepth = MIN_DISTANCE_X - abs(distVec.x);
        float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

        // If both the depths are > 0, then we collided
        if (xDepth > 0 && yDepth > 0) {
            // There was a collision
            return true;
        }
        return false;
    }
}