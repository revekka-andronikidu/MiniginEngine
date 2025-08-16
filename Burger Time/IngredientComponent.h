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
		bool m_EnemiesRegistred{ false };
		bool m_IsWaitingToFall{ false };
        int m_FloorsToDrop{ 0 };
        float m_DelayBeforeFall{ 0.f };
        const GameObject* m_Tray{ nullptr };
        void OnSteppedOn();
        void KillEnemies();
        void Fall();
        bool ShouldFall();
        void RegisterToTray(const GameObject& tray);


    private:
        bool m_PiecesSteppedOn[4]{};
        int m_MinFallDistance{ static_cast<int>(8 * GameSettings::scale.y) }; 
        int m_LastHeight{};
        float m_FallSpeed{ 30.f * GameSettings::scale.y };


        bool m_IsBouncing{ false };
        float m_BounceTimer{ 0.f };
        float m_BounceDuration{ 0.3f };
        float m_BounceHeight{ 4.f * GameSettings::scale.y};     
		float m_BounceStartY{ 0.f };


        std::vector<std::shared_ptr<GameObject>> m_pPieces{};
        std::vector<IngredientPieceComponent*> m_pPiecesComponent{};
		std::vector<GameObject*> m_pRegistredEnemies{};

        void GeneratePieces(IngredientType type);
        std::vector<GameObject*> RegisterEnemies();
        
        friend class IngredientPieceComponent;
    };
}

