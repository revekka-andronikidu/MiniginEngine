#include "TrayComponent.h"
#include "IngredientComponent.h"
#include "GameEvents.h"
#include <GameObject.h>
//#include "EventSystem.h"
//#include <iostream>

using namespace dae;

TrayComponent::TrayComponent(GameObject* owner, int totalPieces)
    : BaseComponent(owner)
    , m_TotalPieces{ totalPieces } 
{
    EventSystem::GetInstance().AddListener(GameEvent::BURGER_PIECE_ON_TRAY, GetOwner(), this);
}

TrayComponent::~TrayComponent()
{
   // dae::EventSystem::GetInstance().RemoveListener(this);
}

void TrayComponent::OnNotify(const GameObject& entity, const Event& event)
{
    if (event == GameEvent::BURGER_PIECE_ON_TRAY)
    {
       /* auto ingredient = entity.GetComponent<IngredientComponent>();
        if (ingredient)
        {
            RegisterIngredient(ingredient);
        }*/
    }
}

void TrayComponent::RegisterIngredient(IngredientComponent* ingredient)
{
    // Avoid duplicates
    for (auto* existing : m_IngredientsOnTray)
    {
        if (existing == ingredient)
            return;
    }

    m_IngredientsOnTray.push_back(ingredient);
    std::cout << "[Tray] Ingredient registered! Total: "
        << m_IngredientsOnTray.size() << "\n";

    CheckIfComplete();
}

void TrayComponent::CheckIfComplete()
{
    if (static_cast<int>(m_IngredientsOnTray.size()) >= m_TotalPieces)
    {
        std::cout << "[Tray] All burgers completed! Bonus points awarded.\n";
        // add 250 points
    }
}