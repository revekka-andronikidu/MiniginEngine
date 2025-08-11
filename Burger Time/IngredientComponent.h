#pragma once
#include <BaseComponent.h>
#include <vector>
#include "Helpers.h"

namespace dae
{
    class IngredientPieceComponent;
    class IngredientComponent final  : public BaseComponent
    {
    public:
        IngredientComponent(dae::GameObject* owner, IngredientType type);
        ~IngredientComponent() = default;

        void Update() override;
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        IngredientComponent(const IngredientComponent& other) = delete;
        IngredientComponent(IngredientComponent&& other) = delete;
        IngredientComponent& operator=(const IngredientComponent& other) = delete;
        IngredientComponent& operator=(IngredientComponent&& other) = delete;


        std::vector<std::shared_ptr<GameObject>> GetPieces() { return m_pPieces; };

    protected:
        bool m_IsFalling{ false };
        bool m_IsOnTheTray{ false };
        void OnSteppedOn();
        void Fall();
        bool ShouldFall();

    private:
        bool m_PiecesSteppedOn[4]{};
        int m_MinFallDistance{ 8 * 3 }; //3 is a scale of the level, get from game settings
        int m_LastHeight{};
        float m_FallSpeed{ 100 };

        std::vector<std::shared_ptr<GameObject>> m_pPieces{};
        std::vector<IngredientPieceComponent*> m_pPiecesComponent{};

        void GeneratePieces(IngredientType type);
        
        friend class IngredientPieceComponent;
    };
}

