#pragma once
#include "Event.h"
#include "GameObject.h"

namespace dae
{
	namespace GameEvent
	{
		//inline const Event LIVES_UPDATED{};
		//inline const Event PLAYER_DEATH{};
		inline const Event POINTS_UPDATED{};
	}

	class PointsIncreasedEvent final : public BaseEvent
	{
	public:
		explicit PointsIncreasedEvent(int points)
			: m_Points(points) {
		}

		int GetPoints() const { return m_Points; }

	private:
		int m_Points;
	};
}