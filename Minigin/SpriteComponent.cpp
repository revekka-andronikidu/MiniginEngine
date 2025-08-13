#include "SpriteComponent.h"
#include "TimeManager.h"


using namespace dae;

SpriteComponent::SpriteComponent(GameObject* pOwner, const std::string& fileName, int rows, int columns, float framesPerSec) : GraphicsComponent(pOwner)
, m_Rows{ rows }
, m_Columns{ columns }
, m_FramesPerSecond{ framesPerSec }
{
	m_pTexture = ResourceManager::GetInstance().GetTexture(fileName);
	m_TotalFrames = m_Rows * m_Columns;
};

void SpriteComponent::Update()
{
    if (m_Animate)
    {
        Animate();
    }
}

glm::ivec2 SpriteComponent::GetTextureSize() const
{
    glm::ivec2 size{ m_pTexture->GetSize().x / m_Columns,  m_pTexture->GetSize().y / m_Rows };
    return size;
}

void SpriteComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const glm::vec3& pos = GetOwner()->GetTransform().GetWorldPosition();
		const glm::vec3& scale = GetOwner()->GetTransform().GetScale();

		Renderer::GetInstance().RenderTexture(*m_pTexture, GetCurrentSrcRect(), pos.x, pos.y, scale, m_IsMirrored);


		//::cout << " rendered" << std::endl;

	}
	else
	{

		//std::cout << " No texture found" << std::endl;
		return;

	}
};

void SpriteComponent::Animate()
{
    m_ElapsedTime += TimeManager::GetInstance().GetDeltaTime();

    if (m_ElapsedTime >= 1.0f/ m_FramesPerSecond)
    {
        m_ElapsedTime -= 1.0f / m_FramesPerSecond;
        
            m_CurrentFrame = (m_CurrentFrame + 1) % m_TotalFrames;
    }
}


SDL_Rect SpriteComponent::GetCurrentSrcRect() const
{
    SDL_Rect srcRect{};
    const int frameWidth = m_pTexture->GetSize().x / m_Columns;
    const int frameHeight = m_pTexture->GetSize().y / m_Rows;
    const int currentRow = m_CurrentFrame / m_Columns;
    const int currentColumn = m_CurrentFrame % m_Columns;

    srcRect.x = currentColumn * frameWidth;
    srcRect.y = currentRow * frameHeight;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    return srcRect;
}