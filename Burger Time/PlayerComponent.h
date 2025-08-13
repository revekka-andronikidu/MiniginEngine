#pragma once
#include <BaseComponent.h>
#include "Helpers.h"

namespace dae
{
	class GridComponent;
	class PlayerComponent final : public BaseComponent
	{
	public:
		PlayerComponent(GameObject* pOwner);
		~PlayerComponent() = default;

		void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;

		void SetGrid(GridComponent* grid) { m_pGrid = grid; };
		void Move(Direction dir);
		void Animate();

	private:
		bool m_IsMoving{ false };
		float m_MoveSpeed{ 50.f * GameSettings::scale.x };
		Direction m_CurrentDirection{ Direction::Down };
		GridComponent* m_pGrid{ nullptr };


	};
}
