#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GraphicsComponent.h"

void dae::GameObject::Update()
{
	for (auto&  component : m_pComponents)
	{
		component->Update();
	}
}
void dae::GameObject::FixedUpdate([[maybe_unused]] const float fixed_time_step)
{
	for (auto& component : m_pComponents)
	{
		component->FixedUpdate(fixed_time_step);
	}

}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		// TODO: register renderables to a list so we don't do this every frame
		auto pRenderable = dynamic_cast<GraphicsComponent*>(component.get());

		if (pRenderable)
			pRenderable->Render();
	}
}


void dae::GameObject::RemoveDeadComponents()
{
	auto removeIfMarkedForRemoval = [](const std::unique_ptr<BaseComponent>& comp)
		{
			return comp->IsMarkedForRemoval();
		};

	const auto [first, last] = std::ranges::remove_if(m_pComponents, removeIfMarkedForRemoval);
	m_pComponents.erase(first, last);
}


void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
