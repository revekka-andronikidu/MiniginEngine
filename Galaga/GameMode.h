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

        virtual void OnEnter() override;
        virtual void Update() override {};
        virtual void OnExit() override {};
        virtual GameModeType GetModeType() const { return GameModeType::Unknown; }

    protected:
        virtual void CreateScenes() = 0;
        bool m_ScenesCreated{ false };

        //hold state?
        //on exit delete states? and scenes

    };

    class GameModeNull final : public GameMode
    {
    public:
        virtual GameModeType GetModeType() const { return GameModeType::Unknown; }
    protected:
        void CreateScenes() override {};

    };

    class SinglePlayerMode final : public GameMode
    {
    public:
        virtual GameModeType GetModeType() const { return GameModeType::Solo; }
    protected:
        void CreateScenes() override;
      
    };

   class CoopMode final : public GameMode
    {
    public:
        virtual GameModeType GetModeType() const { return GameModeType::Coop; }
    protected:
        void CreateScenes() override;
    };

    class VersusMode final : public GameMode
    {
    public:
        virtual GameModeType GetModeType() const { return GameModeType::Versus; }

    protected:
        void CreateScenes() override;
    };

}
