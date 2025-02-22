#pragma once
#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "TimeManager.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner) : BaseComponent(pOwner)
{
	m_pText = pOwner->GetComponent<TextComponent>();
#if _DEBUG
	if (!m_pText)
	{
		std::cout << "Object does not have text component! Text component required for FPS component!" << std::endl;
	}
#endif
}

void dae::FPSComponent::Update()
{
	if (!m_pText)
	{
		
		return;
	}

	m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();


	if (m_accumulatedTime >= m_updateTime)
	{

		std::string fpsText = std::format("{:.1f} FPS", TimeManager::GetInstance().GetFPS());
		m_pText->SetText(fpsText);

		m_accumulatedTime = 0;
	}

}

