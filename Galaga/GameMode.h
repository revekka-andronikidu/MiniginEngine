#pragma once
#include "State.h"

namespace dae
{
    class GameMode : public State
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

    class MainMenuState final : public GameMode
    {
    public:
        void OnEnter() override;
        void Update() override {};
        void OnExit() override;
        GameModeType GetModeType() const { return GameModeType::Unknown; }
    private:
        bool m_menuCreated{ false };

    };

    class HighScoresState final : public GameMode
    {
    public:
        void OnEnter() override;
        void Update() override {};
        void OnExit() override {};
        GameModeType GetModeType() const { return GameModeType::Unknown; }
    private:
        bool m_menuCreated{ false };

    };

    class GameModeNull final : public GameMode
    {
    public:
        virtual void OnEnter() override {};
        virtual GameModeType GetModeType() const { return GameModeType::Unknown; }
    private:
        void CreateScenes() {};
        bool m_ScenesCreated{ false };
    };

    class SinglePlayerMode final : public GameMode
    {
    public:
        virtual void OnEnter() override;
        virtual void Update() override {};
        virtual void OnExit() override {};
        virtual GameModeType GetModeType() const { return GameModeType::Solo; }
    private:
        void CreateScenes();
        bool m_ScenesCreated{ false };
      
    };

   class CoopMode final : public GameMode
    {
    public:
        virtual void OnEnter() override;
        virtual void Update() override {};
        virtual void OnExit() override {};
        virtual GameModeType GetModeType() const { return GameModeType::Coop; }
    private:
        void CreateScenes();
        bool m_ScenesCreated{ false };
    };

    class VersusMode final : public GameMode
    {
    public:
        virtual void OnEnter() override;
        virtual void Update() override {};
        virtual void OnExit() override {};
        virtual GameModeType GetModeType() const { return GameModeType::Versus; }

    private:
        void CreateScenes();
        bool m_ScenesCreated{ false };
    };

    class GameOverState : public GameMode
    {

        virtual void OnEnter();
        virtual void Update() {};
        virtual void OnExit() {};
    };

}
