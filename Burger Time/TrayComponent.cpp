#include "TrayComponent.h"
#include "IngredientComponent.h"
#include "GameEvents.h"
#include <GameObject.h>
#include <EventManager.h>
//#include <iostream>

using namespace dae;

TrayComponent::TrayComponent(GameObject* owner, int totalPieces)
    : BaseComponent(owner)
    , m_TotalPieces{ totalPieces } 
{
   
}

TrayComponent::~TrayComponent()
{
   // dae::EventManager::GetInstance().RemoveListener(this);
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
    //std::cout << "[Tray] Ingredient registered! Total: "
        //<< m_IngredientsOnTray.size() << "\n";

    CheckIfComplete();
}

void TrayComponent::CheckIfComplete()
{
    if (static_cast<int>(m_IngredientsOnTray.size()) >= m_TotalPieces)
    {
        auto event = std::make_shared<dae::PointsIncreasedEvent>(250);
        EventManager::GetInstance().TriggerEvent(*event, *GetOwner());
        EventManager::GetInstance().TriggerEvent(GameEvent::BURGER_COMPLETED, *GetOwner());
    }
}