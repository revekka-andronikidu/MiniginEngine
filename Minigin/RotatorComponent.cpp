#include "RotatorComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include <gtc/constants.hpp>

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, float radius, float period, bool isClockwise) : BaseComponent(pOwner)
, m_Radius{ radius }
, m_RotationSpeed{ period }
, m_IsClockwise{ isClockwise }
{
	m_Origin = pOwner->GetTransform().GetLocalPosition();

}

void dae::RotatorComponent::Update()
{
	if (!m_IsRotating)
	{
		return;
	}

	constexpr float rotation{ 2 * glm::pi<float>() };

	auto clockwise = m_IsClockwise ? 1 : -1;

	m_Angle += TimeManager::GetInstance().GetDeltaTime() * (rotation / m_RotationSpeed) * clockwise;
	m_Angle = glm::mod(m_Angle, rotation);

	if (m_Angle > 360)
	{
		m_Angle -= 360;
	}

	glm::vec3 newPos = { m_Origin + m_Radius * glm::vec3{ glm::cos(m_Angle), glm::sin(m_Angle), 0 } };

	GetOwner()->GetTransform().SetPosition(newPos);

}