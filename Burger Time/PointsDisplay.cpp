#include "PointsDisplay.h"
#include "PointsComponent.h"
#include "ResourceManager.h"
#include <EventManager.h>
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


       dae::EventManager::GetInstance().AddGlobalListener<ScoreUpdatedEvent>(this);
}

dae::PointsDisplay::~PointsDisplay()
{
if (EventManager::IsAlive())
    dae::EventManager::GetInstance().RemoveListener(this);
}


void dae::PointsDisplay::OnNotify(const GameObject& entity, const BaseEvent& event)
{
    if (auto e = dynamic_cast<const ScoreUpdatedEvent*>(&event)) {
        m_CurrentPoints = e->GetNewScore();
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