#pragma once
#include <Singleton.h>
#include <vector>
#include "TrayComponent.h"
#include <EventManager.h>
#include "GameEvents.h"
#include <GameObject.h>


namespace dae
{
	class TrayManager final : public Singleton<TrayManager>, public IEventListener
	{
	public:
		void RegisterTray(TrayComponent* tray)
		{
			m_Trays.push_back(tray);
		}

		void OnNotify(const GameObject& entity, const BaseEvent& event) override
		{
			entity;
			if (dynamic_cast<const BurgerCompletedEvent*>(&event))
			{
				++m_CompletedTrays;
				if (m_CompletedTrays >= m_Trays.size())
				{
					EventManager::GetInstance().TriggerEvent(GameEvent::LEVEL_COMPLETED, dummySender);
				}
			}
		}

		~TrayManager() = default;

		void Reset()
		{
			m_Trays.clear();
			m_CompletedTrays = 0;
		}


	private:
		friend class Singleton<TrayManager>;
		TrayManager()
		{
			//dummySender = std::make_shared<GameObject>();
			EventManager::GetInstance().AddGlobalListener<BurgerCompletedEvent>(this);
		};

		GameObject dummySender;
		std::vector<TrayComponent*> m_Trays;
		int m_CompletedTrays = 0;
	};
}