#pragma once    
#include "LivesDisplay.h"
#include "LivesComponent.h"
#include "ResourceManager.h"
#include "Helpers.h"


dae::LivesDisplay::LivesDisplay(GameObject* owner)
    : GraphicsComponent(owner)
	, m_CurrentLives{ 3 } // Default lives
{
    m_pTexture = ResourceManager::GetInstance().GetTexture("spritesheet.png");
    if (!m_pTexture)
    {
        std::cout << " No texture found" << std::endl;
        //throw
    }

    int sheetWidth{ 30 };
    int sheetHeight{ 22 };
    int posX{ 25 };
    int posY{ 0 };

    auto textureSize = m_pTexture.get()->GetSize();
    int sizeX = textureSize.x / sheetWidth;
    int sizeY = textureSize.y / sheetHeight;

    m_SrcRect = { posX * sizeX, posY * sizeY, sizeX, sizeY };
}

void dae::LivesDisplay::Render() const
{
    if (m_pTexture)
    {
        auto pos = GetOwner()->GetTransform().GetLocalPosition();

            for (int i = 1; i < m_CurrentLives; i++)
            {
                Renderer::GetInstance().RenderTexture(*m_pTexture, m_SrcRect, pos.x, pos.y - (GameSettings::cellSize/2 * GameSettings::scale.y) * i, GameSettings::scale);
            }
    }
}

dae::LivesDisplay::~LivesDisplay()
{
	/*if (EventManager::IsAlive())
	{
		EventManager::GetInstance().RemoveListener(this);
	}*/
}


//void dae::LivesDisplay::OnNotify(const GameObject& entity, const BaseEvent& event)
//{
//
//    //if (event == LIVES_UPDATED_EVENT)
//    //{
//    //    // Update the health bar UI
//    //    m_CurrentLives = entity.GetComponent<LivesComponent>()->GetLives();
//    //    std::cout << "Player Lives: " << std::to_string(m_CurrentLives) << std::endl;
//
//    //}
//
//    //if (event == OBJECT_DEATH_EVENT)
//    //{
//    //    //
//    //    std::cout << "Player Death" << std::endl;
//    //}
//}
