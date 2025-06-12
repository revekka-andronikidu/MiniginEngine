#pragma once
namespace dae
{
    class State
    {
    public:
        virtual ~State() {};

        virtual void OnEnter() = 0;  // Called on enter
        virtual void Update() = 0;  // Called every frame
        virtual void OnExit() = 0;  // Called on exit
    };
}