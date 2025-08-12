#include "TrayComponent.h"
#include "IngredientComponent.h"
#include "GameEvents.h"
#include <GameObject.h>
#include <EventSystem.h>
//#include <iostream>

using namespace dae;

TrayComponent::TrayComponent(GameObject* owner, int totalPieces)
    : BaseComponent(owner)
    , m_TotalPieces{ totalPieces } 
{
   
}

TrayComponent::~TrayComponent()
{
   // dae::EventSystem::GetInstance().RemoveListener(this);
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
        //std::cout << "[Tray] All burgers completed! Bonus points awarded.\n";
        auto event = std::make_shared<dae::PointsIncreasedEvent>(250);
        EventSystem::GetInstance().TriggerEvent(*event, *GetOwner());
    }
}