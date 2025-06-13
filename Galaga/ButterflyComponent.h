#pragma once
#include "EnemyComponent.h"

namespace dae
{
	class ButterflyComponent : public EnemyComponent
	{
	public:
		explicit ButterflyComponent(GameObject* pOwner) : EnemyComponent(pOwner) {};
		~ButterflyComponent() = default;


		void Update() override {};
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

	protected:
		void AddPoints() override;

	};
}

