#pragma once
#include <BookEngine/GameObject.h>
#include <BookEngine/Camera3D.h>
#include <BookEngine/ShaderManager.h>

class BallObject : BookEngine::GameObject
{
public:
	BallObject();
	~BallObject();

	void Init(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
	void Update(float deltaTime);
	void Draw(BookEngine::Camera3D* camera);

	btDefaultMotionState* fallMotionState;
	btScalar mass = 1;
	btCollisionShape* fallShape;
	btRigidBody* fallRigidBody;
private:
	BookEngine::Mesh m_model;
	BookEngine::ShaderManager m_shaderManager;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;

};

