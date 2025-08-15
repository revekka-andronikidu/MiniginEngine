#pragma once
#include <BaseComponent.h>
#include "Helpers.h"
#include <EventManager.h>

namespace dae
{
	class GridComponent;
	class EnemyComponent : public BaseComponent, public IEventListener
	{
	public:
		EnemyComponent(GameObject* pOwner);
		~EnemyComponent() = default;
		void Update() override {};

		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;

		void SetGrid(GridComponent* grid) { m_pGrid = grid; };
		void OnNotify(const GameObject& entity, const BaseEvent& event) override;
		//void Move(Direction dir);
		//void Animate();

	private:
		bool m_IsMoving{ false };
		int m_Points{}; // Points awarded for defeating this enemy
		float m_MoveSpeed{ 45.f * GameSettings::scale.x };
		Direction m_CurrentDirection{ Direction::Down };
		GridComponent* m_pGrid{ nullptr };

	};
}
