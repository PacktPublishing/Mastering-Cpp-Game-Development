#include "NPC.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


NPC::NPC()
{
}

NPC::~NPC()
{
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;
	delete fallShape;

}




void NPC::Init(const glm::vec3& pos, const glm::vec3& scale)
{
	// Create and compile our GLSL program from the shaders
	m_shaderManager.CompileShaders("Shaders/SimpleObjectShader.vert", "Shaders/SimpleShader.frag");
	m_shaderManager.AddAttribute("vertexPosition_modelspace");
	m_shaderManager.AddAttribute("vertexColor");
	m_shaderManager.LinkShaders();

	m_model.Init("Meshes/Ball.obj", "Textures/Ball.png");

	fallShape = new btSphereShape(10);


	m_position = pos;
	btVector3 fallInertia(0.0f, 0.0f, 0.0f);

	fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(m_position.x, m_position.y, m_position.z)));

	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);


	m_model.Init("Meshes/Ball.obj", "Textures/Ball.png");

	// Translation
	m_modelMatrix = glm::translate(m_modelMatrix, pos);
	m_modelMatrix = glm::scale(m_modelMatrix, scale);
}

void NPC::Update(float deltaTime)
{
	
	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);
	m_position.x = trans.getOrigin().getX();
	m_position.y = trans.getOrigin().getY();
	m_position.z = trans.getOrigin().getZ();
	// Translation
	m_modelMatrix = glm::translate(glm::mat4(1), m_position);
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	
}

void NPC::Draw(BookEngine::Camera3D * camera)
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
