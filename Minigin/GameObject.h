#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include <iostream>
#include <typeinfo> 
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		virtual void Update();
		virtual void FixedUpdate(const float fixed_time_step);
		virtual void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		Transform& GetTransform() { return m_transform; };

		#pragma region Component system
		
				template <typename ComponentType, typename... Args>
				ComponentType* AddComponent(Args&&... args) 
				{
					static_assert(std::is_base_of_v<BaseComponent, ComponentType>,
						"ComponentType must derive from BaseComponent");


					// Check if a component of the exact type already exists
					for (const auto& component : m_pComponents) 
					{
						if (typeid(*component.get()) == typeid(ComponentType)) 
						{
#if _DEBUG
							std::cout << "Component of type " << typeid(ComponentType).name() << " already exists!" << std::endl;
#endif
							return nullptr;
						}
					}

					

					// Create and add the new component
					auto newComponent = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);
					ComponentType* rawPtr = newComponent.get(); // Store the raw pointer before moving ownership to avoid undefined behavior
					m_pComponents.push_back(std::move(newComponent));	
#if _DEBUG
					//std::cout << "Component of type " << typeid(ComponentType).name() << " added!" << std::endl;
#endif
					return rawPtr;
				}
		
				template<typename ComponentType>
				ComponentType* GetComponent() const
				{
					for (auto& component : m_pComponents)
					{
						auto pComp = dynamic_cast<ComponentType*>(component.get());
						if (pComp)
						{
							return pComp;
						}
					}
					return nullptr;
				}
		
				template <typename ComponentType>
				bool HasComponent()
				{
					return GetComponent<ComponentType>() != nullptr;
				}
		
				//marks component for removal and removes all components as a last step of update in the scene after everything else was updated
				template<typename ComponentType>
				void RemoveComponent()
				{
					for (auto& component : m_pComponents)
					{
						auto pComp = dynamic_cast<ComponentType*>(component.get());
						if (pComp)
						{
							component->MarkForRemoval();
						}
					}
				}

				void RemoveDeadComponents();
		
		#pragma endregion

	private:

		Transform m_transform;
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;

	};

}