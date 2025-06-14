#include "PlayerComponent.h"
#include "EngineEvents.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "SceneManager.h"
#include "LivesComponent.h"
#include "ServiceLocator.h"


using namespace dae;

void dae::PlayerComponent::OnNotify(const GameObject& entity, const Event& event)
{

    if (event->GetType() == typeid(EngineEvents::Hit) )
    {
        const auto& data = static_cast<const TypedEvent<EngineEvents::Hit>&>(*event).GetData();

        if (data.source->HasTag("enemyBullet") || data.target->HasTag("enemyBullet") || data.source->HasTag("enemy") || data.target->HasTag("enemy"))
        {
            std::cout << "YAYAYAYAYY" << std::endl;

            if (data.source->HasTag("enemyBullet"))
                data.source->Destroy(); 
            else if (data.target->HasTag("enemyBullet"))
                data.target->Destroy();

            //if (data.source->HasTag("enemy"))
            //{}//data.source->Destroy(); create function for enemy death
            //else if (data.target->HasTag("enemy"))
            //{}   //data.target->Destroy();

           
                //auto explosion = ObjectFactory::GetInstance().CreatePlayerExplosion(GetOwner()->GetTransform().GetWorldPosition());
                //SceneManager::GetInstance().GetActiveScene().Add(std::move(explosion));

               
            
            OnHit();
            
            // animation
            // 
            //temp disapear and respawn

        }
    }
}

void PlayerComponent::OnHit()
{
    ServiceLocator::GetAudioService().PlayEffect(2, 0.8, true);
    //animation
    //deactivate and respawn
    GetOwner()->GetComponent<LivesComponent>()->RemoveLive();
}