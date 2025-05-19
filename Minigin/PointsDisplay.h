#pragma once
#include "GraphicsComponent.h"
#include "Observer.h"
#include "TextComponent.h"


namespace dae
{
    class PointsDisplay final : public GraphicsComponent, public Observer
    {
    public:
        explicit PointsDisplay(GameObject* pOwner);
        ~PointsDisplay() {};

        void Render() const override;
        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void OnNotify(const GameObject& entity, const Event& event) override;

    private:
        int m_CurrentPoints;
        TextComponent* m_pText;


    };
};

