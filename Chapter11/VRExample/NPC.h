#pragma once
#include <string>
#include <vector>
#include <sol/sol.hpp>
#include <BookEngine/IGameObject.h>
#include <BookEngine/Camera3D.h>
#include <BookEngine/ShaderManager.h>

class NPC : BookEngine::IGameObject
{
	
public:
	NPC();
	~NPC();

	void Init(const glm::vec3& pos, const glm::vec3& scale);
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
	sol::state lua;
};
