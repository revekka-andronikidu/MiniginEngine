#pragma once
#include "SpriteComponent.h"	
#include <unordered_map>

namespace dae
{
    struct Animation
    {
        int startFrame;  // Starting frame index in the sprite sheet
        int frameCount;  // How many frames in this animation
        float framesPerSecond;
    };

    class SpriteSheetComponent final : public SpriteComponent
    {
    public:
        SpriteSheetComponent(GameObject* pOwner, const std::string& fileName, int rows, int columns);
        ~SpriteSheetComponent() = default;

        void SetAnimation(const std::string& animName, bool isMirrored = false);
        void AddAnimation(const std::string& animName, const Animation& anim);


    private:
        //float m_ElapsedTime{};
       // int m_CurrentFrameIndex{};
        Animation* m_pCurrentAnimation{ nullptr };

        std::unordered_map<std::string, Animation> m_Animations;

        std::string m_CurrentState;
        virtual void Animate() override;
        SDL_Rect GetCurrentSrcRect() const override;
       
    };

}

