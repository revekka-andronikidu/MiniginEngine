#include "TimeManager.h"
namespace dae
{
    void TimeManager::Init()
    {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_lastFrameTime = m_startTime;


    }

    void TimeManager::Update()
    {
        // Get the current time and calculate the delta time since the last frame
        auto currentTime = std::chrono::high_resolution_clock::now();
        m_deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
        m_lastFrameTime = currentTime;

        auto elapsedSeconds = std::chrono::duration<float>(currentTime - m_startTime).count();
        ++m_FrameCount;

        if (elapsedSeconds >= 1.0f)
        {
            m_FPS = m_FrameCount / elapsedSeconds;

            // Reset counters
            m_FrameCount = 0;
            m_startTime = currentTime;
        }
    }

   
}