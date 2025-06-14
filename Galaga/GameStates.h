#pragma once
#include <State.h>
#include <string>


namespace dae
{
    class GameState : public State
    {
    public:
        virtual void OnEnter() override {};
        virtual void Update() override {};
        virtual void OnExit() override {};
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

    class WaveOneState : public InGameState
    {
    public:
        virtual void OnEnter();
        virtual void Update() {};
        virtual void OnExit() {};

    private:
        void CreateScene(std::string name);
        bool m_Created{ false };
   
    };





 

    


    
    //class PostLevelState : public InGameState
    //{

    //    virtual void OnEnter();
    //    virtual void Update();
    //    virtual void OnExit();
    //};

    //paused
}
