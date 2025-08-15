#pragma once
#include <BaseComponent.h>
#include "Helpers.h"
#include <EventManager.h>

namespace dae
{
	class GridComponent;
	class PlayerComponent final : public BaseComponent, IEventListener
	{
	public:
		PlayerComponent(GameObject* pOwner);
		~PlayerComponent();

		void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;

		void SetGrid(GridComponent* grid) { m_pGrid = grid; };
		void Move(Direction dir);
		void Pepper();
		void OnNotify(const GameObject& entity, const BaseEvent& event) override;
		void ResetPlayer(glm::vec3 pos);
		void StopMovement() { m_IsMoving = false; m_CanMove = false; }
		void AllowMovement() { m_CanMove = true; }

	private:
		bool m_IsMoving{ false };
		bool m_IsDead{ false };
		bool m_AnimSet{ false };
		bool m_CanMove{ true };
		float m_MoveSpeed{ 50.f * GameSettings::scale.x };
		float m_TimeSinceLastPepper{ 0.f };
		float m_PepperCooldown{ 0.5f }; // Cooldown for pepper usage
		Direction m_CurrentDirection{ Direction::Down };
		GridComponent* m_pGrid{ nullptr };

		void Animate();
		void SpawnPepper();
	};
}
