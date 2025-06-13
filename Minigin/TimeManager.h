#pragma once
#include <chrono>
#include "Singleton.h"

namespace dae
{
    class TimeManager final : public Singleton<TimeManager>
    {
    public:
        TimeManager() { Init(); };
        void Init();
        void Update();

        float GetDeltaTime() const { return static_cast<float>(m_deltaTime); };
        float GetFPS() const { return static_cast<float>(m_FPS); };
        int GetFixedFrameRate() const { return m_fixedUpdateFramerate;  } ;
        std::chrono::time_point<std::chrono::high_resolution_clock> GetLastFrameTime() { return m_lastFrameTime; };

    protected:

        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;


        const int m_fixedUpdateFramerate{60};
        float m_deltaTime = 0; // The time elapsed since the last frame        
        float m_FPS = 0;
        int m_FrameCount = 0;

  
    };


}