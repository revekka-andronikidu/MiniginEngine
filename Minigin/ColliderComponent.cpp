#pragma once
#include "ColliderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EngineEvents.h"
#include "Subject.h"
#include "EventSystem.h"

using namespace dae;
void ColliderComponent::UpdateOverlaps()
{ 
    if (!m_IsActive) return;
    m_CurrentOverlaps.clear();

	auto objects = SceneManager::GetInstance().GetActiveScene().GetObjectsInScene();

    for (auto* other : objects)
    {
        if (!other || other == GetOwner() || other->IsMarkedForDestroy()) //check if object is valid
            continue;

        const auto otherCollider = other->GetComponent<ColliderComponent>();
        if (!otherCollider)  //check if object has collider
            continue;

        if (!otherCollider->m_IsActive) //check if collider is active
            continue;


        if (IsOverlapping(otherCollider))
        {
            m_CurrentOverlaps.insert(other);

            if (!m_PreviousOverlaps.contains(other) && GetOwner() < other)
            {
                //std::cout << "OVERLAP STARTED" << std::endl;

               /* Notify(*other, EngineEvent::COLLISION); 
                otherCollider->Notify(*GetOwner(), EngineEvent::COLLISION);*/
                EventSystem::GetInstance().TriggerEvent(CollisionEvent(*other), *GetOwner());
                EventSystem::GetInstance().TriggerEvent(CollisionEvent(*GetOwner()), *other);
            }
        }
    }

        //// Check for overlaps that ended
        //for (auto* previous : m_PreviousOverlaps)
        //{
        //    if (!m_CurrentOverlaps.contains(previous) && GetOwner() < previous)
        //    {
        //        //std::cout << "OVERLAP ENDED" << std::endl;
        //    }
        //}

        // Save current as previous for next frame
        m_PreviousOverlaps = m_CurrentOverlaps;

	
}

bool ColliderComponent::IsOverlapping(const ColliderComponent* other)
{
    if (!other) return false;

    // Get position of both colliders
    const glm::vec3 posA = GetOwner()->GetTransform().GetWorldPosition() + m_Offset;
    const glm::vec3 posB = other->GetOwner()->GetTransform().GetWorldPosition() + other->m_Offset;

    const glm::vec3 sizeA = m_Size;
    const glm::vec3 sizeB = other->m_Size;

    // Compute AABB bounds for each collider (min and max corners)
    const glm::vec3 minA = posA;
    const glm::vec3 maxA = posA + sizeA;

    const glm::vec3 minB = posB;
    const glm::vec3 maxB = posB + sizeB;

    // Perform AABB overlap check on X and Y (and optionally Z if 3D)
    const bool overlapX = minA.x < maxB.x && maxA.x > minB.x;
    const bool overlapY = minA.y < maxB.y && maxA.y > minB.y;
   // const bool overlapZ = minA.z < maxB.z && maxA.z > minB.z; // use if you want 3D

    // Return true if overlapping in 2D (X and Y)
    return overlapX && overlapY;
}