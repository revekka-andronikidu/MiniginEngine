#pragma once
#include "ColliderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EngineEvents.h"
#include "Subject.h"
#include "EventManager.h"

using namespace dae;
void ColliderComponent::UpdateOverlaps()
{ 
    if (!m_IsActive) return;
    m_CurrentOverlaps.clear();

	auto objects = SceneManager::GetInstance().GetActiveScene().GetObjectsInScene();

    for (auto* other : objects)
    {
        if (!other || other == GetOwner() || other->IsMarkedForDestroy()) 
            continue;

        const auto otherCollider = other->GetComponent<ColliderComponent>();
        if (!otherCollider)  
            continue;

        if (!otherCollider->m_IsActive) 
            continue;


        if (IsOverlapping(otherCollider))
        {
            m_CurrentOverlaps.insert(other);

            if (!m_PreviousOverlaps.contains(other) && GetOwner() < other)
            {
                EventManager::GetInstance().TriggerEvent(CollisionEvent(*other), *GetOwner());
                EventManager::GetInstance().TriggerEvent(CollisionEvent(*GetOwner()), *other);
            }
        }
    }
        m_PreviousOverlaps = m_CurrentOverlaps;	
}

bool ColliderComponent::IsOverlapping(const ColliderComponent* other)
{
    if (!other) return false;

    const glm::vec3 posA = GetOwner()->GetTransform().GetWorldPosition() + m_Offset;
    const glm::vec3 posB = other->GetOwner()->GetTransform().GetWorldPosition() + other->m_Offset;

    const glm::vec3 sizeA = m_Size;
    const glm::vec3 sizeB = other->m_Size;

    const glm::vec3 minA = posA;
    const glm::vec3 maxA = posA + sizeA;

    const glm::vec3 minB = posB;
    const glm::vec3 maxB = posB + sizeB;

    const bool overlapX = minA.x < maxB.x && maxA.x > minB.x;
    const bool overlapY = minA.y < maxB.y && maxA.y > minB.y;

    return overlapX && overlapY;
}