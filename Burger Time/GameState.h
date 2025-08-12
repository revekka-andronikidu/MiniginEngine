#pragma once
#include <State.h>

namespace dae
{

    class GameState : public State
    {
    public:

        enum class GameModeType
        {
            Solo,
            Coop,
            Versus,
            Unknown
        };

        virtual void OnEnter() override {};
        virtual void Update() override {};
        virtual void OnExit() override {};
        virtual GameModeType GetModeType() const { return GameModeType::Unknown; }
    };

    class MainMenuState final : public GameState
    {
    public:
        void OnEnter() override;
        void Update() override {};
        void OnExit() override;
        GameModeType GetModeType() const { return GameModeType::Unknown; }
    private:
        bool m_menuCreated{ false };

    };

    class SinglePlayerMode final : public GameState
    {
    public:
        void OnEnter() override;
        void Update() override {};
        void OnExit() override {};
        GameModeType GetModeType() const { return GameModeType::Solo; }

        void NextStage();
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


    };
}