#include "EnemyComponent.h"
#include "EngineEvents.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "ColliderComponent.h"

using namespace dae;

void dae::EnemyComponent::OnNotify(const GameObject& entity, const Event& event)
{

    if (event->GetType() == typeid(EngineEvents::Hit))
    {
        const auto& data = static_cast<const TypedEvent<EngineEvents::Hit>&>(*event).GetData();

        if (data.source->HasTag("bullet") || data.target->HasTag("bullet"))
        {
            OnHit();
        }
    }

    if (event == LIVES_UPDATED_EVENT)
    {
        // Update the health bar UI
       auto m_CurrentLives = entity.GetComponent<LivesComponent>()->GetLives();
        std::cout << "Enemy Lives: " << std::to_string(m_CurrentLives) << std::endl;

    }
    else if (event == OBJECT_DEATH_EVENT)
    {
        OnDeath();
        std::cout << "Enemy Death" << std::endl;
    }
}

void dae::EnemyComponent::OnHit()
{
    //register hit for statistics
    GetOwner()->GetComponent<LivesComponent>()->RemoveLive();
}
void dae::EnemyComponent::OnLooseLife()
{

}
void dae::EnemyComponent::OnDeath()
{
    //register points to player
    
    GetOwner()->GetComponent<ColliderComponent>()->m_IsActive = false;
    //play animation then destroy

    GetOwner()->Destroy();

}

