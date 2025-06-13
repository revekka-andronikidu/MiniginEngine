#pragma once
#include "EnemyComponent.h"

namespace dae
{
	class BossGalagaComponent : public EnemyComponent
	{
	public:
		explicit BossGalagaComponent(GameObject* pOwner) : EnemyComponent(pOwner) {};
		~BossGalagaComponent() = default;


		void Update() override {};
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

	protected:
		void AddPoints() override;

	};
}

