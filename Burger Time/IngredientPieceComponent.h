#pragma once
#include <GraphicsComponent.h>
#include "Helpers.h"
#include <EventManager.h>
//#include <Observer.h>


namespace dae
{
    class IngredientComponent;
	class IngredientPieceComponent final : public GraphicsComponent, public IEventListener
	{
    public:
        IngredientPieceComponent(dae::GameObject* owner, IngredientType type, int piece, IngredientComponent& parent);
        ~IngredientPieceComponent();

        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};
        void Render() const override;

        IngredientPieceComponent(const IngredientPieceComponent& other) = delete;
        IngredientPieceComponent(IngredientPieceComponent&& other) = delete;
        IngredientPieceComponent& operator=(const IngredientPieceComponent& other) = delete;
        IngredientPieceComponent& operator=(IngredientPieceComponent&& other) = delete;

        void OnNotify(const GameObject& entity, const BaseEvent& event) override;

        bool IsSteppedOn() { return m_SteppedOn; };
        void IncrementNudge();
        void Reset();

        
        
    
    private:
        SDL_Rect m_SrcRect;
        bool m_SteppedOn;
        int m_Nudge{0};
        float m_NudgeSize{ 1 };
        IngredientComponent& m_ParentIngredient;

	};

}

