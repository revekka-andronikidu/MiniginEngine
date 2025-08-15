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

void SpriteSheetComponent::SetAnimation(const std::string& animName, bool isMirrored, const std::string& nextAnim)
{
    if (animName == m_CurrentState && isMirrored == m_IsMirrored) return;

    m_CurrentState = animName;
    m_IsMirrored = isMirrored;
    m_NextAnimation = nextAnim; // store the next animation to play (can be empty)

    auto it = m_Animations.find(m_CurrentState);
    if (it != m_Animations.end())
    {
        m_pCurrentAnimation = &it->second;
        m_CurrentFrame = 0;
        m_ElapsedTime = 0.f;
		m_AnimationFinished = false; 
    }
}

//void SpriteSheetComponent::SetAnimation(const std::string& animName, bool isMirrored)
//{
//    if (animName == m_CurrentState && isMirrored == m_IsMirrored) return;
//
//    m_CurrentState = animName;
//    m_IsMirrored = isMirrored;
//
//    auto it = m_Animations.find(animName);
//    if (it != m_Animations.end())
//    {
//        m_pCurrentAnimation = &it->second;
//        m_CurrentFrame = 0;
//        m_ElapsedTime = 0.f;
//    }
//}


void SpriteSheetComponent::Animate()
{
    if (!m_pCurrentAnimation) return;
    if (m_pCurrentAnimation->frames.empty()) return;

    m_ElapsedTime += TimeManager::GetInstance().GetDeltaTime();
    if (m_ElapsedTime >= 1.0f / m_pCurrentAnimation->framesPerSecond)
    {
        m_ElapsedTime -= 1.0f / m_pCurrentAnimation->framesPerSecond;
        m_CurrentFrame++; //= (m_CurrentFrame + 1) % m_pCurrentAnimation->frameCount;

        if (m_CurrentFrame >= m_pCurrentAnimation->frames.size())
        {
            if (!m_NextAnimation.empty())
            {
                // Play the next animation once current finishes
                SetAnimation(m_NextAnimation, m_IsMirrored);
                m_NextAnimation.clear();
            }
            else if (!m_pCurrentAnimation->loopAnimation)
            {
                m_CurrentFrame = (int)m_pCurrentAnimation->frames.size() - 1;
                m_AnimationFinished = true; 
            }
            else
            {
                m_CurrentFrame = 0;
            }
        }
    }
}

SDL_Rect SpriteSheetComponent::GetCurrentSrcRect() const
{
    if (!m_pCurrentAnimation || m_pCurrentAnimation->frames.empty()) return {};

    int absoluteFrame = m_pCurrentAnimation->frames[m_CurrentFrame];

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


