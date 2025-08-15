#pragma once
#include <State.h>
#include "Helpers.h"

namespace dae
{

    class GameState : public State
    {
    public:

        virtual void OnEnter() override {};
        virtual void Update() override {};
        virtual void OnExit() override {};
        virtual GameMode GetModeType() const { return GameMode::Unknown; }
        void NextStage();
    };

    class InGameState : public GameState
    {
    public:
        InGameState(GameMode mode)
            : m_Mode(mode) {}


        virtual GameMode GetModeType() const { return m_Mode; }
        void OnEnter() override;
		void OnExit() override;

    private:
        GameMode m_Mode;
    };

    class MainMenuState final : public GameState
    {
    public:
        void OnEnter() override;
        void OnExit() override;

        GameMode GetModeType() const { return GameMode::Unknown; }
    };

    class LevelCompleteState : public GameState
    {
    public:
        LevelCompleteState(float waitTime) : m_WaitTime(waitTime), m_Timer{0.f} {}

        void OnEnter() override;
        void Update() override;
        void OnExit() override;

    private:
        float m_Timer{0.f};
		float m_WaitTime{ 3.0f }; // wait time before next stage
    };

    class PlayerDeathState : public GameState
    {
    public:
        PlayerDeathState(float waitTime) : m_WaitTime(waitTime), m_Timer{ 0.f } {}

        void OnEnter() override;
        void Update() override;
        void OnExit() override;

    private:
        float m_Timer{ 0.f };
        float m_WaitTime{ 3.0f }; // wait time before next stage
    };

    class GameOverState : public GameState
    {
    public:
        GameOverState(float waitTime) : m_WaitTime(waitTime), m_Timer{ 0.f } {}

        void OnEnter() override;
        void Update() override;
        void OnExit() override;

    private:
        float m_Timer{ 0.f };
        float m_WaitTime{ 3.0f }; // wait time before next stage
        void NextScreen();
    };

   

    class HighScoresState : public GameState
    {
    public:
        void OnEnter() override;
        void OnExit() override;
    };


    class HighScoreEnterState : public GameState
    {
    public:
        void OnEnter() override;
        void OnExit() override;
    };







   /* class SinglePlayerMode final : public GameState
    {
    public:
        void OnEnter() override;
        void Update() override {};
        void OnExit() override {};
        GameModeType GetModeType() const { return GameModeType::Solo; }

        
    private:


    };

    class CoopMode final : public GameState
    {
    public:
        void OnEnter() override {};
        void Update() override {};
        void OnExit() override {};
        GameModeType GetModeType() const { return GameModeType::Solo; }
    private:


    };

    class VersusMode final : public GameState
    {
    public:
        void OnEnter() override {};
        void Update() override {};
        void OnExit() override {};
        GameModeType GetModeType() const { return GameModeType::Solo; }
    private:


    };*/
}