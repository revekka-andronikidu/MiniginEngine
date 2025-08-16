#pragma once
#include <BaseComponent.h>
#include "Helpers.h"
#include <EventManager.h>

namespace dae
{
	class GridComponent;
	struct CellPos;
	class EnemyComponent : public BaseComponent, public IEventListener
	{
	public:
		EnemyComponent(GameObject* pOwner);
		~EnemyComponent() = default;
		void Update() override;

		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;

		void SetGrid(GridComponent* grid) { m_pGrid = grid; };
		void OnNotify(const GameObject& entity, const BaseEvent& event) override;
		void Squish();
		void Die();
		void RegisterToIngredient(GameObject* burger);

		bool IsActive() const { return !m_IsPeppered && !m_IsDead && !m_IsRegisteredToBurger; };
		bool CanBeRegistered() const { return !m_IsDead && !m_IsSquished && !m_IsRegisteredToBurger; };
		bool m_CanMove{ true };

	private:
		bool m_IsPeppered{ false };
		bool m_IsSquished{ false };
		bool m_IsDead{ false };
		bool m_IsRegisteredToBurger{ false };
		int m_Points{}; // Points awarded for defeating this enemy
		float m_PepperedTimer{0.f};
		const float m_PepperedDuration{ 2.f };
		const float m_MoveSpeed{ 30.f * GameSettings::scale.x };
		bool m_CalculatePath{ true };
		Direction m_CurrentDirection{ Direction::Down };
		GridComponent* m_pGrid{ nullptr };


		std::vector<CellPos> m_CurrentPath;
		int m_NextCellIndex{ 1 };

		void Pepper();
		void UnPepper();
		Direction CalculateDirection();
		void Move();
		void FollowPath();
		void Animate();
		bool CanGoLeft();
		bool CanGoRight();
		bool CanGoUp();
		bool CanGoDown();
		//void SetRandomDirection();
		
	};
}
