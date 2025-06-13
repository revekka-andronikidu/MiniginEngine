#pragma once
#include "EnemyComponent.h"

namespace dae
{
	class BeeComponent : public EnemyComponent
	{
	public:
		explicit BeeComponent(GameObject* pOwner) : EnemyComponent(pOwner) {};
		~BeeComponent() = default;


		void Update() override {};
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};
	
	protected:
		void AddPoints() override;

	};
}

