#pragma once
#include "BaseComponent.h"



namespace dae {


	class PointsComponent final : public BaseComponent
	{
	public:
		PointsComponent(GameObject* pOwner, int points = 0);
		~PointsComponent() = default;

		void IncreasePoints(int points);
		int GetPoints() const { return m_Points; };

		void Update() override {};
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};


	private:
		int m_Points = 0;
	};
}

