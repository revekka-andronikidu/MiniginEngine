#include "PointsComponent.h"
#include "GameEvents.h"

namespace dae
{
	PointsComponent::PointsComponent(GameObject* owner, int points)
		: BaseComponent(owner)
		, Subject()
		, m_Points(points)
	{
	}

	void PointsComponent::IncreasePoints(int points)
	{
		m_Points += points;
		Notify(*GetOwner(), POINTS_UPDATED_EVENT);

	}
}