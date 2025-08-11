#include "EnemyComponent.h"
#include "EngineEvents.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "ColliderComponent.h"
#include "GalagaGame.h"
#include "ServiceLocator.h"


using namespace dae;

void dae::EnemyComponent::OnNotify(const GameObject& entity, const Event& event)
{

    if (event->GetType() == typeid(EngineEvents::Collision))
    {
        const auto& data = static_cast<const TypedEvent<EngineEvents::Collision>&>(*event).GetData();

        if (data.source->HasTag("bullet"))
        { 
            data.source->Destroy(); // Destroy the bullet
            OnHit();
        }
        else if (data.target->HasTag("bullet"))
        {
            data.target->Destroy(); // Destroy the bullet
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

    auto * galaga = dynamic_cast<GalagaGame*>(GameManager::GetInstance().GetActiveGame());
    galaga->IncrementEnemiesHit();
    GetOwner()->GetComponent<LivesComponent>()->RemoveLive();
}
void dae::EnemyComponent::OnLooseLife()
{

}
void dae::EnemyComponent::OnDeath()
{
    //register points to player
    //Create enemy explosion
    AddPoints();
       
    ServiceLocator::GetAudioService().PlayEffect(3, 0.8f, false);
        
        // Add to killed enemies counter
       // GameMaster::GetInstance().IncrementKilledEnemies();
    
    GetOwner()->GetComponent<ColliderComponent>()->m_IsActive = false;
    //play animation then destroy

    GetOwner()->Destroy();

}

