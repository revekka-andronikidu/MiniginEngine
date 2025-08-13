#include "SpriteSheetComponent.h"
#include "TimeManager.h"

using namespace dae;

SpriteSheetComponent::SpriteSheetComponent(GameObject* pOwner, const std::string& fileName, int rows, int columns)
    : SpriteComponent(pOwner, fileName, rows, columns)
{
}

void SpriteSheetComponent::AddAnimation(const std::string& animName, const Animation& anim)
{
    m_Animations[animName] = anim;
}

void SpriteSheetComponent::SetAnimation(const std::string& animName, bool isMirrored)
{
    if (animName == m_CurrentState && isMirrored == m_IsMirrored) return;

    m_CurrentState = animName;
    m_IsMirrored = isMirrored;

    auto it = m_Animations.find(animName);
    if (it != m_Animations.end())
    {
        m_pCurrentAnimation = &it->second;
        m_CurrentFrame = 0;
        m_ElapsedTime = 0.f;
    }
}


void SpriteSheetComponent::Animate()
{
    if (!m_pCurrentAnimation) return;

    m_ElapsedTime += TimeManager::GetInstance().GetDeltaTime();
    if (m_ElapsedTime >= 1.0f / m_pCurrentAnimation->framesPerSecond)
    {
        m_ElapsedTime -= 1.0f / m_pCurrentAnimation->framesPerSecond;
        m_CurrentFrame = (m_CurrentFrame + 1) % m_pCurrentAnimation->frameCount;
    }
}

SDL_Rect SpriteSheetComponent::GetCurrentSrcRect() const
{
    if (!m_pCurrentAnimation) return {};

    int absoluteFrame = m_pCurrentAnimation->startFrame + m_CurrentFrame;
    const int frameWidth = m_pTexture->GetSize().x / m_Columns;
    const int frameHeight = m_pTexture->GetSize().y / m_Rows;
    const int row = absoluteFrame / m_Columns;
    const int col = absoluteFrame % m_Columns;

    SDL_Rect srcRect;
    srcRect.x = col * frameWidth;
    srcRect.y = row * frameHeight;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    return srcRect;
}


