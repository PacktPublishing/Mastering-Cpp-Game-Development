#include "GroundObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


GroundObject::GroundObject()
{
}


GroundObject::~GroundObject()
{
}

void GroundObject::Init(const glm::vec3& pos, const glm::vec3& scale)
{
	// Create and compile our GLSL program from the shaders
	m_shaderManager.CompileShaders("Shaders/SimpleObjectShader.vert", "Shaders/SimpleShader.frag");
	m_shaderManager.AddAttribute("vertexPosition_modelspace");
	m_shaderManager.AddAttribute("vertexColor");
	m_shaderManager.LinkShaders();

	m_position = pos;
	groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(m_position.x, m_position.y, m_position.z)));


	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);

	m_model.Init("Meshes/Floor.obj", "Textures/floor.png");

	// Translation
	m_modelMatrix = glm::translate(m_modelMatrix, pos);
	m_modelMatrix = glm::scale(m_modelMatrix, scale);
}

void GroundObject::Draw(BookEngine::Camera3D* camera)
{
	//Use shader
	m_shaderManager.Use();
	// Send our transformation to the currently bound shader - our MVP
	GLint model = m_shaderManager.GetUniformLocation("Model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	GLint view = m_shaderManager.GetUniformLocation("View");
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->GetView()));
	GLint projection = m_shaderManager.GetUniformLocation("Projection");
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera->GetProjection()));

	GLint textureUniform2 = m_shaderManager.GetUniformLocation("TextureSampler");
	glUniform1i(textureUniform2, 0);
	m_model.Draw();
	m_shaderManager.UnUse();
}
