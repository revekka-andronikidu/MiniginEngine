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


	protected:
		float m_ShootInterval{ 1.15f };
		float m_ShootTimer{ 0 };
		void Shoot();

	};

}