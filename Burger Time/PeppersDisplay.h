#pragma once
#include "GraphicsComponent.h"
#include <EventManager.h>

namespace dae
{
	class TextComponent;

    class PeppersDisplay final : public GraphicsComponent, public IEventListener
    {
    public:
        explicit PeppersDisplay(GameObject* pOwner);

        ~PeppersDisplay();

        void Render() const override;
        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void OnNotify(const GameObject& entity, const BaseEvent& event) override {};

        void SetPeppers(int peppers);
    private:
        int m_CurrentPeppers;
        SDL_Rect m_SrcRect;
		TextComponent* m_pText;
    };
};

