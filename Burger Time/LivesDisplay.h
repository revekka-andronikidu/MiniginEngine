#pragma once
#include "GraphicsComponent.h"

namespace dae
{
    class LivesDisplay final : public GraphicsComponent
    {
    public:

        //explicit LivesDisplay(GameObject* pOwner);
        explicit LivesDisplay(GameObject* pOwner);

        ~LivesDisplay();

        void Render() const override;
        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		void SetLives(int lives)
		{
			m_CurrentLives = lives;
		}
        //void OnNotify(const GameObject& entity, const BaseEvent& event) override;

    private:
        int m_CurrentLives;
		SDL_Rect m_SrcRect;
    };
};

