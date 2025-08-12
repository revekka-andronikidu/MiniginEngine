#include "PointsDisplay.h"
#include "PointsComponent.h"
#include "ResourceManager.h"
#include <EventSystem.h>
#include "GameEvents.h"

dae::PointsDisplay::PointsDisplay(GameObject* owner, std::string text)
    : GraphicsComponent(owner)
    , m_CurrentPoints{0}
    , m_textString{text}
{
    if (owner->GetComponent<TextComponent>() == nullptr)
    {
        auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);
        std::string text{ m_textString + std::to_string(m_CurrentPoints) };
        owner->AddComponent<TextComponent>(text, font, SDL_Color{255,255,255,255}, TextComponent::TextAlign::Right);

    }
    m_pText = owner->GetComponent<TextComponent>();


        dae::EventSystem::GetInstance().AddGlobalListener<PointsIncreasedEvent>(this);
}

dae::PointsDisplay::~PointsDisplay()
{
if (EventSystem::IsAlive())
    dae::EventSystem::GetInstance().RemoveListener(this);
}


void dae::PointsDisplay::OnNotify(const GameObject& entity, const BaseEvent& event)
{
    if (auto pointsEvent = dynamic_cast<const PointsIncreasedEvent*>(&event))
    {
        m_CurrentPoints += pointsEvent->GetPoints();
        UpdateDisplay();
    }

}

void dae::PointsDisplay::UpdateDisplay()
{
    m_pText->SetText(m_textString + std::to_string(m_CurrentPoints));
}

void dae::PointsDisplay::Render() const
{   
    m_pText->Render();
}