#pragma once
#include "Command.h"
#include "GameObject.h"
#include "TimeManager.h"


using namespace dae;

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(GameObject* gameObject, glm::vec3 direction, float speed = 10.f) : GameObjectCommand(gameObject), m_Direction{ direction }, m_MoveSpeed{ speed } {};

	void Execute() override
	{
		auto transform = GetGameObject()->GetTransform();
		auto translation = m_Direction * m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime() + transform.GetLocalPosition();
		GetGameObject()->GetTransform().SetPosition(translation);

		glm::vec3 pos = translation;
		std::cout << "Moved to: ("
			+ std::to_string(pos.x) + ", "
			+ std::to_string(pos.y) + ", "
			+ std::to_string(pos.z) + ")"
			<< std::endl;

	};
	void SetDirection(glm::vec3 direction) { m_Direction = direction; };
	void SetSpeed(float speed) { m_MoveSpeed = speed; };

private:
	glm::vec3 m_Direction{};
	float m_MoveSpeed{};
};