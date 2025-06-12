#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <type_traits>

namespace dae
{
	template<typename TState>
	class StateMachine
	{
	public:
		static_assert(std::is_polymorphic<TState>::value, "TState must be a polymorphic base class (have virtual functions)");

		template<typename TDerivedState, typename... Args>
		void EnterState(Args&&... args)
		{
			static_assert(std::is_base_of<TState, TDerivedState>::value, "State must derive from base state type");

			const std::type_index id = std::type_index(typeid(TDerivedState));

			if (m_StateCache.find(id) == m_StateCache.end())
			{
				m_StateCache[id] = std::make_unique<TDerivedState>(std::forward<Args>(args)...);
			}

			if (m_CurrentState)
				m_CurrentState->OnExit();

			m_CurrentState = m_StateCache[id].get();
			m_CurrentState->OnEnter();
		}

		void Update()
		{
			if (m_CurrentState)
				m_CurrentState->Update();
		}

		TState* GetCurrentState() const { return m_CurrentState; }

	private:
		std::unordered_map<std::type_index, std::unique_ptr<TState>> m_StateCache;
		TState* m_CurrentState{ nullptr };
	};
}
