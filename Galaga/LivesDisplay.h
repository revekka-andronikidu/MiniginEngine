#pragma once
#include "GraphicsComponent.h"
#include "Observer.h"
#include "GameEvents.h"

namespace dae
{
    class LivesDisplay final : public GraphicsComponent, public Observer
    {
    public:

        //explicit LivesDisplay(GameObject* pOwner);
        explicit LivesDisplay(GameObject* pOwner, const std::string& filePath,const  glm::vec3 scale = glm::vec3(1, 1, 1), const bool mirorred = false);

        ~LivesDisplay();

        void Render() const override;
        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void OnNotify(const GameObject& entity, const Event& event) override;

    private:
        int m_CurrentLives;
        bool m_IsMirrored;
        glm::vec3 m_Scale;
    };
};

