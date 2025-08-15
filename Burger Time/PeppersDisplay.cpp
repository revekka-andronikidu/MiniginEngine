#pragma once    
#include "PeppersDisplay.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Helpers.h"
#include "GameEvents.h"
#include "BurgerTimeGame.h"

dae::PeppersDisplay::PeppersDisplay(GameObject* owner)
    : GraphicsComponent(owner)
    , m_CurrentPeppers{ 5 } // Default lives
{
    m_pTexture = ResourceManager::GetInstance().GetTexture("spritesheet.png");
    if (!m_pTexture)
    {
        std::cout << " No texture found" << std::endl;
        //throw
    }

    int sheetWidth{ 30 };
    int sheetHeight{ 22 };
    int posX{ 27 };
    int posY{ 1 };

    auto textureSize = m_pTexture.get()->GetSize();
    int sizeX = textureSize.x / sheetWidth;
    int sizeY = textureSize.y / sheetHeight;

    m_SrcRect = { posX * sizeX, posY * sizeY, sizeX * 3, sizeY };

    std::string text{ std::to_string(m_CurrentPeppers) };
    auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);

    if (owner->GetComponent<TextComponent>() == nullptr)
    {
        m_pText = owner->AddComponent<TextComponent>(text, font, SDL_Color{ 255,255,255,255 }, TextComponent::TextAlign::Right);  
    }
    else
    {
        m_pText = owner->GetComponent<TextComponent>();
		m_pText->SetText(text);
		m_pText->SetAlign(TextComponent::TextAlign::Right);
		m_pText->SetColor({ 255, 255, 255, 255 });
        m_pText->SetFont(font);
    }

    m_pText->SetVerticalOffset(GameSettings::cellSize/2 * GameSettings::scale.y);

    dae::EventManager::GetInstance().AddGlobalListener<PeppersChangedEvent>(this);
}

dae::PeppersDisplay::~PeppersDisplay()
{
	if (EventManager::IsAlive())
	{
		dae::EventManager::GetInstance().RemoveListener(this);
	}
}

void dae::PeppersDisplay::Render() const
{
    if (m_pTexture)
    {
       auto pos = GetOwner()->GetTransform().GetLocalPosition();

       Renderer::GetInstance().RenderTexture(*m_pTexture, m_SrcRect, pos.x - (m_SrcRect.w* GameSettings::scale.x), pos.y, GameSettings::scale);
	   m_pText->Render();
        
    }
}

void dae::PeppersDisplay::SetPeppers(int peppers)
{
    m_CurrentPeppers = peppers;
    m_pText->SetText(std::to_string(m_CurrentPeppers));
}

void dae::PeppersDisplay::OnNotify(const GameObject& entity, const BaseEvent& event)
{
	if (auto evemt = dynamic_cast<const PeppersChangedEvent*>(&event))
	{
        auto game = GameManager::GetInstance().GetActiveGame();
        auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

        m_CurrentPeppers = burgerTime->m_Peppers;

		std::cout << "Player Peppers: " << std::to_string(m_CurrentPeppers) << std::endl;
		m_pText->SetText(std::to_string(m_CurrentPeppers));
	}
}
