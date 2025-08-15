#pragma once
#include <BaseComponent.h>

namespace dae
{
	enum class Direction;
	class PepperComponent final : public BaseComponent
	{
	public:
		PepperComponent(GameObject* pOwner, Direction direction);
		~PepperComponent() = default;

		void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};
		

	private:
		Direction m_Direction;
		void SetAnimation();
	};
}

