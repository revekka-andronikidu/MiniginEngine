#pragma once
#include "SpriteComponent.h"	
#include <unordered_map>

namespace dae
{
    struct Animation
    {
        std::vector<int> frames;  
        float framesPerSecond;
        bool loopAnimation = true;
    };

    class SpriteSheetComponent final : public SpriteComponent
    {
    public:
        SpriteSheetComponent(GameObject* pOwner, const std::string& fileName, int rows, int columns);
        ~SpriteSheetComponent() = default;

        void SetAnimation(const std::string& animName, bool isMirrored = false, const std::string& nextAnim = "");
        void AddAnimation(const std::string& animName, const Animation& anim);
        bool IsAnimationFinished() const { return m_AnimationFinished; };

    private:
		bool m_AnimationFinished{ false };
        Animation* m_pCurrentAnimation{ nullptr };

        std::unordered_map<std::string, Animation> m_Animations;
        std::string m_NextAnimation;
        std::string m_CurrentState;
        virtual void Animate() override;
        SDL_Rect GetCurrentSrcRect() const override;
       
    };

}

