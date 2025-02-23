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


void dae::GameObject::RemoveDeadComponents()
{
	auto removeIfMarkedForRemoval = [](const std::unique_ptr<BaseComponent>& comp)
		{
			return comp->IsMarkedForRemoval();
		};

	const auto [first, last] = std::ranges::remove_if(m_pComponents, removeIfMarkedForRemoval);
	m_pComponents.erase(first, last);
}

void dae::GameObject::SetParent(GameObject* newParent)
{
	//check if new parent is valid
	if (!IsValidParent(newParent))
		return;


	//Store world transform before changing hierarchy
	const glm::mat4 oldWorldMatrix = m_transform.GetWorldMatrix();

	//Calculate new local transform based on new parent
	if (m_pParent)
	{
		//m_transform.SetPosition(m_transform.GetWorldPosition() - newParent->GetTransform().GetWorldPosition());
		
		const glm::mat4 parentWorldInverse = glm::inverse(m_pParent->m_transform.GetWorldMatrix());
		const glm::mat4 newLocalMatrix = parentWorldInverse * oldWorldMatrix;
		m_transform.SetLocalMatrix(newLocalMatrix);
	}
	else
	{
		//m_transform.SetPosition(m_transform.GetWorldPosition());
		m_transform.SetLocalMatrix(oldWorldMatrix);
	}

	// Remove from pervious parent
	if (m_pParent)
		m_pParent->RemoveChild(this);

	// Set new parent
	m_pParent = newParent;
	if (m_pParent)
		m_pParent->AddChild(this);//add itself as a child to given parent

	m_transform.SetDirty();
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

void dae::GameObject::RemoveChild(GameObject* childToRemove)
{
	if (childToRemove == nullptr) //Check child is valid 
		return;

	auto find = std::find(m_pChildren.begin(), m_pChildren.end(), childToRemove);
	if (find != m_pChildren.end())
	{
		m_pChildren.erase(find); //Remove the given child from the children list
		childToRemove->SetParent(nullptr); //Remove itself as a parent of the child
	}
}


void dae::GameObject::AddChild(GameObject* childToAdd)
{
	//Check if the new child is valid
	if (childToAdd == nullptr)
	{
#if _DEBUG
		std::cout << "Cannot add child that is nullptr!\n";
#endif
		return;
	}
		

	m_pChildren.push_back(childToAdd);
}
