#pragma once
#include <BookEngine/IGameObject.h>
#include <BookEngine/ShaderManager.h>
#include <BookEngine/Camera3D.h>

class GroundObject : BookEngine::IGameObject
{
public:
	GroundObject();
	~GroundObject();

    void Init(const glm::vec3& pos = glm::vec3(0.0f,0.0f,0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
	void Draw(BookEngine::Camera3D* camera);

	btCollisionShape* groundShape = nullptr;
	btDefaultMotionState* groundMotionState = nullptr;
	btRigidBody* groundRigidBody = nullptr;

private:
	BookEngine::Mesh m_model;
	BookEngine::ShaderManager m_shaderManager;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;

};

