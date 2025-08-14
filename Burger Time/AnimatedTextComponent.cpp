#include "AnimatedTextComponent.h"
#include <TimeManager.h>
#include "TextComponent.h"

using namespace dae;

AnimatedTextComponent::AnimatedTextComponent(GameObject* pOwner, TextComponent* pText, std::string fullText, float interval)
	: BaseComponent(pOwner)
	, m_pText(pText)
	, m_interval(interval)
	, m_fullText(std::move(fullText))
{
}

void AnimatedTextComponent::Update()
{
        if (!m_active || !m_pText) return;

        m_timer += TimeManager::GetInstance().GetDeltaTime();

        // Add next letter when interval reached
        if (m_timer >= m_interval && m_index < m_fullText.size())
        {
            m_displayed += m_fullText[m_index++];
            m_pText->SetText(m_displayed);
            m_timer = 0.f;
        }

        // Finished displaying
        if (m_index >= m_fullText.size())
            m_active = false; 
}


