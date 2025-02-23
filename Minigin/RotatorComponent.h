#pragma once
#include "BaseComponent.h"
#include <glm.hpp>

namespace dae
{
	class Transform;

	class RotatorComponent final : public BaseComponent
	{
	public:
		RotatorComponent(GameObject* pOwner, float radius = 20.f, float period = 1.f, bool isClockwise = true);

		~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		void ToggleRotation() { m_IsRotating = !m_IsRotating; };

	private:
		bool m_IsClockwise;
		bool m_IsRotating{ true };

		float m_RotationSpeed;
		float m_Radius;
		float m_Angle{};

		glm::vec3 m_Origin;
	};
}

