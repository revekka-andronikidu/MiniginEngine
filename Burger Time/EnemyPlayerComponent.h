#pragma once
#include <BaseComponent.h>
#include <EventManager.h>
#include <glm.hpp>
#include "Helpers.h"

namespace dae
{
	class GridComponent;    
    enum class Direction;
	class EnemyPlayerComponent : public BaseComponent, IEventListener
    {


    public:
        EnemyPlayerComponent(GameObject* owner);
		~EnemyPlayerComponent();

		void Update() override;
        void OnNotify(const GameObject& entity, const BaseEvent& event) override;

        void Squish();
        void Die();
        void RegisterToIngredient(GameObject* burger);
		void Move(Direction dir);
		void SetGrid(GridComponent* grid) { m_pGrid = grid; };
        void ResetPlayer(glm::vec3 pos);
 
    private:
        bool m_IsPeppered{ false };
		bool m_IsMoving{ false };
        bool m_IsSquished{ false };
        bool m_IsDead{ false };
        bool m_IsRegisteredToBurger{ false };
        bool m_HasReachedSpawn{ false };
        bool m_CanMove{ false };
        int m_Points{ 1000 };
        glm::vec3 m_SpawnPosition;
		GridComponent* m_pGrid{ nullptr };
		float m_MoveSpeed{ 35.f * GameSettings::scale.y}; // Speed of the enemy player
        Direction m_CurrentDirection{};
        void Animate();
        void  WalkIn();
		void StopMovement() { m_IsMoving = false; m_CanMove = false; }

    };
}
