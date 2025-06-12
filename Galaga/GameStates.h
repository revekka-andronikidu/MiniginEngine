#pragma once
#include <State.h>

namespace dae
{
    class GameState : public State
    {
    public:
        virtual void OnEnter() override {};
        virtual void Update() override {};
        virtual void OnExit() override {};
    };


    class MainMenuState final : public GameState
    {
    public: 
        void OnEnter() override;
        void Update() override {};
        void OnExit() override;

    private:
        bool m_menuCreated{ false };
       
    };

   class InGameState : public GameState
    {

       virtual void OnEnter() {};
       virtual void Update() {};
       virtual void OnExit() {};
    };

 /*  class StageScreenState : public InGameState
   {
   public:
       virtual void OnEnter();
       virtual void Update() {};
       virtual void OnExit() {};
   private:

      void CreateScene();
   };*/

    class StageOneState : public InGameState
    {
    public:
        virtual void OnEnter();
        virtual void Update() {};
        virtual void OnExit() {};
    private:
        void CreateScene();
    };

    class StageTwoState : public InGameState
    {

        virtual void OnEnter() ;
        virtual void Update() {};
        virtual void OnExit() {};
    };

    class StageThreeState : public InGameState
    {

        virtual void OnEnter() ;
        virtual void Update() {};
        virtual void OnExit() {};
    };




    class GameOverState : public GameState
    {

        virtual void OnEnter() {};
        virtual void Update() {};
        virtual void OnExit() {};
    };

    


    
    //class PostLevelState : public InGameState
    //{

    //    virtual void OnEnter();
    //    virtual void Update();
    //    virtual void OnExit();
    //};

    //paused
}
