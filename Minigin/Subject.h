#pragma once
#include <vector>
#include "Observer.h"
#include "GameObject.h"

namespace dae
{
	class Subject
	{
	public:
		//Subject() : m_pHead(NULL) {};
		Subject() : m_pObservers{} {};
		virtual ~Subject() = default;

		Subject(const Subject&) = delete;
		Subject(Subject&&) noexcept = delete;
		Subject& operator= (const Subject&) = delete;
		Subject& operator= (const Subject&&) noexcept = delete;

		//void AddObserver(Observer* observer)
		//{
		//	observer->m_pNext = m_pHead;
		//	m_pHead = observer;
		//}
		//
		//void RemoveObserver(Observer* observer)
		//{
		//	Observer** current = &m_pHead; // Pointer to pointer to traverse the list
		//
		//	while (*current) // Dereference to get the actual pointer
		//	{
		//		if (*current == observer) // Found the observer to remove
		//		{
		//			*current = observer->m_pNext; // Bypass the observer
		//			observer->m_pNext = nullptr;  // Nullify removed node's next pointer
		//			return;
		//		}
		//		current = &((*current)->m_pNext); // Move to next observer
		//	}
		//
		//	/*if (m_pHead == observer)
		//	{
		//		m_pHead = observer->m_pNext;
		//		observer->m_pNext = NULL;
		//		return;
		//	}
		//
		//	Observer* current = m_pHead;
		//	while (current != NULL)
		//	{
		//		if (current->m_pNext == observer)
		//		{
		//			current->m_pNext = observer->m_pNext;
		//			observer->m_pNext = NULL;
		//			return;
		//		}
		//
		//		current = current->m_pNext;
		//	}*/
		//}
		//
		//void RemoveAllObservers()
		//{
		//	Observer* current = m_pHead;
		//	while (current != NULL)
		//	{
		//		Observer* next = current->m_pNext;
		//		delete current;
		//		current = next;
		//	}
		//	m_pHead = NULL;
		//}

		void AddObserver(Observer* const observer)
		{
			m_pObservers.emplace_back(observer);
		}

		void RemoveObserver(Observer* const observer)
		{
			auto it = std::find(m_pObservers.begin(), m_pObservers.end(), observer);
			if (it != m_pObservers.end())
				m_pObservers.erase(it);
		}

		void RemoveAllObservers()
		{
			m_pObservers.clear();
		}



	protected:
		/*void Notify(const GameObject& entity, Event event)
		{
			Observer* observer = m_pHead;
			while (observer != NULL)
			{
				observer->OnNotify(entity, event);
				observer = observer->m_pNext;
			}
		}*/

		void Notify(const GameObject& entity, const Event& event)
		{
			for (auto observer : m_pObservers)
			{
				observer->OnNotify(entity, event);
			}
		}
	private:
		//Observer* m_pHead;
		std::vector<Observer*> m_pObservers{};
	};
};
