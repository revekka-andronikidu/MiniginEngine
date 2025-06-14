#pragma once 
#include "BaseComponent.h"
namespace dae
{
	class EnemyAttackComponent : public BaseComponent
	{
	public:
		EnemyAttackComponent(dae::GameObject* gameObject) : BaseComponent(gameObject) {};
		virtual ~EnemyAttackComponent() = default;

		virtual void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		void StartAttack();
		void StopAttack();

	protected:
		bool m_IsAttacking{ false };
		int m_ShotsPerAttack{ 2 };
		int m_ShotsFired{ 0 };
		float m_ShootInterval{ 0.25f };
		float m_ShootTimer{ 0 };

		void Shoot();

	};

}