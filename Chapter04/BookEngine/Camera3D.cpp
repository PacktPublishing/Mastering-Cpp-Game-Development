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
#include "Camera3D.h"
#include <SDL/SDL.h>
namespace BookEngine
{


	Camera3D::Camera3D()
	{
	}


	Camera3D::~Camera3D()
	{
	}

	void Camera3D::Init(glm::vec3 cameraPosition, float horizontalAngle, float verticalAngle, float initialFoV)
	{
		m_position = cameraPosition;
		m_horizontalAngle = horizontalAngle;
		m_verticalAngle = verticalAngle;
		m_initialFoV = initialFoV;

		Update();

	}
	void Camera3D::Update()
	{
		
		// Direction : Spherical coordinates to Cartesian coordinates conversion
		m_forward = glm::vec3(
			glm::cos(m_verticalAngle) * glm::sin(m_horizontalAngle),
			glm::sin(m_verticalAngle),
			glm::cos(m_verticalAngle) * glm::cos(m_horizontalAngle)
		);

		// Right vector
		m_right = glm::vec3(
			sin(m_horizontalAngle - 3.14f / 2.0f),
			0,
			cos(m_horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		m_up = glm::cross(m_right, m_forward);


		float FoV = m_initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		m_projection = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

		// Camera matrix
		m_view = glm::lookAt(
			m_position,           // Camera is here
			m_position + m_forward, // and looks here : at the same position, plus "direction"
			m_up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

		m_MVP = m_projection * m_view * m_model; // Remember, matrix multiplication is the other way around

	}
}
