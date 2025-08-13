#pragma once
#include "GraphicsComponent.h"
#include "TextComponent.h"
#include <EventManager.h>


namespace dae
{
    class PointsDisplay final : public GraphicsComponent, public IEventListener
    {
    public:
        explicit PointsDisplay(GameObject* pOwner, std::string text = {""});
        ~PointsDisplay();

        void Render() const override;
        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void OnNotify(const GameObject& entity, const BaseEvent& event) override;

		void SetPoints(int points)
		{
			m_CurrentPoints = points;
			UpdateDisplay();
		}

    private:
        int m_CurrentPoints;
        std::string m_textString;
        TextComponent* m_pText;

       void UpdateDisplay();


    };
};

