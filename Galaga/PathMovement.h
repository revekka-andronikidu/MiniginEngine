#pragma once
#include "BaseComponent.h"
#include <vector>
#include <vec2.hpp>

namespace dae
{
	class PathMovement : public BaseComponent
	{
	public:
		PathMovement(dae::GameObject* gameObject, const std::vector<glm::vec2>& path = {}, float speed = 100.0f);

		void Update() override;
		//void SetPath(const std::vector<glm::vec2>& path);
		void StartAtFirstPoint();
		void AddWorldSpacePoint(const glm::vec2& point);

		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

	private:
		void MoveAlongPath();
		glm::vec2 ConvertToWorldCoordinates(const glm::vec2& relativePos);

		std::vector<glm::vec2> m_Path;
		float m_Speed;
		size_t m_CurrentPoint;
		bool m_PathComplete;
		
	};

}

