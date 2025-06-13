#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class PlayerComponent final : public BaseComponent, public Observer
	{
    public:
        //explicit LivesDisplay(GameObject* pOwner);
        explicit PlayerComponent(GameObject* pOwner) : BaseComponent(pOwner) {};
        ~PlayerComponent() = default;


        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void OnNotify(const GameObject& entity, const Event& event) override;
    private:
        void OnHit();

	};

}

