#pragma once
#include <BaseComponent.h>

namespace dae
{
	class EnemyPlayerComponent : public BaseComponent, IEventListener
    {


    public:
        void Squish();
        void Die();
        void RegisterToIngredient(GameObject* burger);
		void Move(Direction dir);
 
    private:
        bool m_IsPeppered{ false };
        bool m_IsSquished{ false };
        bool m_IsDead{ false };
        bool m_IsRegisteredToBurger{ false };
        bool m_HasReachedSpawn{ false };
        bool m_CanMove{ false };
        int m_Points{ 1000 };
		float m_MoveSpeed{ 35.f * GameSettings::scale.y}; // Speed of the enemy player
        Direction m_CurrentDirection{};
        void Animate();
        void  WalkIn();
    };
}
