#pragma once
#include "BaseComponent.h"
#include <functional>

namespace dae
{
    class ActionComponent final : public BaseComponent
    {
    public:
        ActionComponent(GameObject* owner, std::function<void()> action) : BaseComponent(owner), m_Action(std::move(action)) 
        {};

        ~ActionComponent() = default;

        void Excecute() 
        {
            if (m_Action) m_Action();
        }

        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

    private:
        std::function<void()> m_Action;
    };

}
