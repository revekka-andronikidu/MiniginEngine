#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GraphicsComponent.h"

dae::GameObject::~GameObject()
{
	// Remove from parent's children
	if (m_pParent)
	{
		m_pParent->m_pChildren.erase(
			std::remove(m_pParent->m_pChildren.begin(), m_pParent->m_pChildren.end(), this),
			m_pParent->m_pChildren.end()
		);
	}

	// Orphan all children and update their transforms
	for (auto* child : m_pChildren)
	{
		child->m_pParent = nullptr;
	}
	//std::cout << "DESTRUCTOR" << std::endl;
}

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

void dae::GameObject::Destroy()
{
	//SetParent(nullptr);
	m_IsMarkedForDestroy = true;
	RemoveAllComponents();
	// Copy the children to avoid modifying the container while iterating
	const auto childrenCopy = m_pChildren;
	for (auto& child : childrenCopy)
	{
		if (child) child->Destroy();
	}

	// Clear children after destroying
	m_pChildren.clear();
}

void dae::GameObject::RemoveAllComponents()
{
	for (auto& component : m_pComponents)
	{
		component->MarkForRemoval();
	}
}

void dae::GameObject::RemoveDeadComponents()
{
	auto removeIfMarkedForRemoval = [](const std::unique_ptr<BaseComponent>& comp)
		{
			if (comp->IsMarkedForRemoval())
			{
				//comp->~BaseComponent();  // Optional: if your BaseComponent has cleanup logic
				return true;
			}
			return false;
		};

	const auto [first, last] = std::ranges::remove_if(m_pComponents, removeIfMarkedForRemoval);
	m_pComponents.erase(first, last);
}

void dae::GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
{
	//check if new parent is valid
	if (!IsValidParent(newParent))
		return;

	//Calculate new local transform based on new parent
	if (newParent == nullptr)
	{
		//oldParent->m_Children.erase(std::remove(oldParent->m_Children.begin(), oldParent->m_Children.end(), shared_from_this()), oldParent->m_Children.end());
		glm::vec3 worldPos = m_transform.GetWorldPosition();
		m_transform.SetPosition(worldPos);
	}
	else
	{
		if (keepWorldPosition)
		{
			m_transform.SetPosition(m_transform.GetLocalPosition() - newParent->m_transform.GetWorldPosition());
		
		}
		
	}

	// Remove from pervious parent
	if(m_pParent)
	m_pParent->m_pChildren.erase(std::remove(m_pParent->m_pChildren.begin(), m_pParent->m_pChildren.end(), this), m_pParent->m_pChildren.end());


	// Set new parent
	m_pParent = newParent;

	//Add itself as a child to the given parent
	if (m_pParent)
		m_pParent->m_pChildren.push_back(this);
	
}

bool dae::GameObject::IsValidParent(GameObject* newParent)
{
	//check that the parent is not itself or already a parent
	if (newParent == this)
	{
#if _DEBUG
		std::cout << "Cannot parent object to itself!\n";
#endif
		return false;
	}

	//check that the new parent is not one of the children
	if (IsChild(newParent))
	{
#if _DEBUG
		std::cout << "Cannot parent object to its child, circular parenting!\n";
#endif
		return false;
	}
	return true;
}

bool dae::GameObject::IsChild(GameObject* parent)
{
	auto isChildFunc = [parent](auto child) { return child == parent || child->IsChild(parent); };

	auto found{ std::ranges::find_if(m_pChildren, isChildFunc) };

	if (found == m_pChildren.end()) //if child not found the new parent is not alredy a child
		return false;

	else return true;
}

void dae::GameObject::SetTag(const std::string& tag)
{
	m_Tag = tag;
}

const std::string& dae::GameObject::GetTag() const
{
	return m_Tag;
}

bool dae::GameObject::HasTag(const std::string& tag) const
{
	return m_Tag == tag;
}





