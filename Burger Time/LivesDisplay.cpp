#include "LivesDisplay.h"
#include "LivesComponent.h"
#include "ResourceManager.h"
#include <iostream>



//dae::LivesDisplay::LivesDisplay(GameObject* owner)
//    : GraphicsComponent(owner)
//    , m_IsMirrored{ false }
//    , m_CurrentLives{}
//{
//}

dae::LivesDisplay::LivesDisplay(GameObject* owner, const std::string& filePath, const glm::vec3 scale, const bool mirrored)
    : GraphicsComponent(owner)
    , m_IsMirrored{ mirrored }
    , m_CurrentLives{}
    , m_Scale{scale}
{


    m_pTexture = ResourceManager::GetInstance().LoadTexture(filePath);


}

void dae::LivesDisplay::Render() const
{
    if (m_pTexture)
    {
        auto pos = GetOwner()->GetTransform().GetLocalPosition();
        auto textureSize = m_pTexture.get()->GetSize();
    

        float offset = 5.f;

        if (m_IsMirrored)
        {
            for (int i = 0; i <= m_CurrentLives - 2; i++)
            {
                Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x - (textureSize.x * m_Scale.x) * i - offset * i, pos.y, m_Scale);
            }
        }
        else
        {
            for (int i = 0; i <= m_CurrentLives - 2; i++)
            {
                Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x + (textureSize.x * m_Scale.x) * i + offset * i, pos.y, m_Scale);
            }
        }
    }
}

dae::LivesDisplay::~LivesDisplay()
{
    // if (m_pLivesComponent)
     //{
         //m_pLivesComponent->RemoveAllObservers();
        // m_pLivesComponent = nullptr;
     //}
}


void dae::LivesDisplay::OnNotify(const GameObject& entity, const Event& event)
{

    //if (event == LIVES_UPDATED_EVENT)
    //{
    //    // Update the health bar UI
    //    m_CurrentLives = entity.GetComponent<LivesComponent>()->GetLives();
    //    std::cout << "Player Lives: " << std::to_string(m_CurrentLives) << std::endl;

    //}

    //if (event == OBJECT_DEATH_EVENT)
    //{
    //    //
    //    std::cout << "Player Death" << std::endl;
    //}
}
