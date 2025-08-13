#pragma once
#include "Event.h"
#include "GameObject.h"

namespace dae
{

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

	class ScoreUpdatedEvent final : public BaseEvent
	{
	public:
		explicit ScoreUpdatedEvent(int score)
			: m_Score(score) {
		}

		int GetNewScore() const { return m_Score; }

	private:
		int m_Score;
	};

	class PeppersChangedEvent final : public BaseEvent
	{
	public:
		explicit PeppersChangedEvent(int peppers)
			: m_Peppers(peppers) {
		}

		int GetPeppers() const { return m_Peppers; }

	private:
		int m_Peppers;
	};

	class BurgerCompletedEvent final : public BaseEvent
	{	
	public:
		explicit BurgerCompletedEvent() {};
				
	};

	class LevelCompleteEvent final : public BaseEvent
	{
	public:
		explicit LevelCompleteEvent() {};

	};

	namespace GameEvent
	{
		//inline const Event LIVES_UPDATED{};
		inline const BurgerCompletedEvent BURGER_COMPLETED{};
		inline const LevelCompleteEvent LEVEL_COMPLETED{};
	}
}