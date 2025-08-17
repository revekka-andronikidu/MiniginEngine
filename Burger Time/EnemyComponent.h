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
		EnemyComponent(GameObject* pOwner, int points, EnemyType type);
		~EnemyComponent();
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
		bool m_CanMove{ false };
		EnemyType GetType() const { return m_Type; };

	private:
		bool m_IsPeppered{ false };
		bool m_IsSquished{ false };
		bool m_IsDead{ false };
		bool m_IsRegisteredToBurger{ false };
		bool m_HasReachedSpawn{ false };
		bool m_CalculatePath{ true };
		int m_Points{}; // Points awarded for defeating this enemy
		float m_PepperedTimer{ 0.f };
		const float m_PepperedDuration{ 2.f };
		const float m_MoveSpeed{ 25.f * GameSettings::scale.x };
		glm::vec3 m_SpawnPosition{0,0,0};

		
		Direction m_CurrentDirection{ Direction::Down };
		GridComponent* m_pGrid{ nullptr };
		const EnemyType m_Type{ EnemyType::MrHotDog }; // Default type, can be set in constructor

		std::vector<glm::vec3> m_CurrentPath;
		std::vector<glm::vec3> m_NewPath;
		glm::vec3 m_TargetPosition{ 0,0,0 };
		int m_CurrentPathPoint{1};

		void Pepper();
		void UnPepper();
		glm::vec3 GetAnchorPosition() const;
		void Animate();
		void WalkIn();
		void Activate();

		bool MoveTowardsTarget(glm::vec3 target);
		void SetNewTarget();
		std::vector<glm::vec3> CalculatePath();
		bool IsSameDirection(Direction currentDir, const glm::vec3& currentTarget, const glm::vec3& nextTarget) const;
		bool IsValidNextTarget(Direction currentDir, const glm::vec3& currentTarget, const glm::vec3& nextTarget) const;
		//void SetRandomDirection();
		
	};
}
