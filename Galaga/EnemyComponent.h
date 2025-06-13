#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "Subject.h"


namespace dae
{
    class EnemyComponent : public BaseComponent, public Observer, public Subject 
    {
    public:
        //explicit LivesDisplay(GameObject* pOwner);
        explicit EnemyComponent(GameObject* pOwner): BaseComponent(pOwner){};
        ~EnemyComponent() = default;


        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void OnNotify(const GameObject& entity, const Event& event) override;

    private:
        void OnHit();
        void OnLooseLife();
        void OnDeath();
        
    };

}


