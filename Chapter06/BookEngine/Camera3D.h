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
#include "InputManager.h"

namespace BookEngine
{
	class Camera3D
	{
	public:
		Camera3D();
		~Camera3D();
		void Init(glm::vec3 cameraPosition = glm::vec3(4,10,10),
				  float horizontalAngle = -2.0f, 
				  float verticalAngle = 0.0f, 
				  float initialFoV = 45.0f);
		void Update();

		glm::mat4 GetMVPMatrix() { return m_MVP; };

		glm::mat4 GetView() { return m_view; };
		glm::mat4 GetProjection() { return m_projection; };
		glm::vec3 GetForward() { return m_forward; };
		glm::vec3 GetRight() { return m_right; };
		glm::vec3 GetUp() { return m_up; };

		void SetHorizontalAngle(float angle) { m_horizontalAngle = angle; };
		void SetVerticalAngle(float angle) { m_verticalAngle = angle; };

		void MoveCamera(glm::vec3 movementVector) { m_position += movementVector; };
		

	private:
		glm::mat4 m_model = glm::mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 m_projection;
		glm::mat4 m_view; // Camera matrix
		glm::mat4 m_MVP; // Our ModelViewProjection : multiplication of our 3 matrices
		
		glm::vec3 m_position;
		float m_horizontalAngle;
		float m_verticalAngle;
		float m_initialFoV;

		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::vec3 m_forward;

		float m_speed = 0.5f;
		float m_mouseSpeed = 0.005f;
	};
}
