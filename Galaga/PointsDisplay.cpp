#include "PointsDisplay.h"
#include "PointsComponent.h"
#include "ResourceManager.h"
#include "GameEvents.h"

dae::PointsDisplay::PointsDisplay(GameObject* owner, std::string text)
    : GraphicsComponent(owner)
    , m_CurrentPoints{}
    , m_textString{text}
{
    if (owner->GetComponent<TextComponent>() == nullptr)
    {
        auto font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
        std::string text{ m_textString + std::to_string(m_CurrentPoints) };
        owner->AddComponent<TextComponent>(text, font);

    }
    m_pText = owner->GetComponent<TextComponent>();


}


void dae::PointsDisplay::OnNotify(const GameObject& entity, const Event& event)
{
    if (event == POINTS_UPDATED_EVENT)
    {
        m_CurrentPoints = entity.GetComponent<PointsComponent>()->GetPoints();
    }

}

void dae::PointsDisplay::Render() const
{
    m_pText->SetText(m_textString + std::to_string(m_CurrentPoints));
    auto pos = GetOwner()->GetTransform().GetLocalPosition();

    m_pText->Render();

}