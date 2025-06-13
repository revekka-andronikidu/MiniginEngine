#pragma once
#include "ColliderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EngineEvents.h"
#include "Subject.h"


using namespace dae;
void ColliderComponent::UpdateOverlaps()
{ 
    if (!m_IsActive) return;
    m_CurrentOverlaps.clear();

	auto objects = SceneManager::GetInstance().GetActiveScene().GetObjectsInScene();

    for (auto* other : objects)
    {
        if (other == GetOwner() || other->IsMarkedForDestroy() || !other) //check if object is valid
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

                // Create event
                auto hitEvent = std::make_shared<TypedEvent<EngineEvents::Hit>>(EngineEvents::Hit{ GetOwner(), other });

                Notify(*GetOwner(), hitEvent);                  // This collider’s observers
                otherCollider->Notify(*other, hitEvent);
                // Notify both objects — assuming they are Subjects
                //GetOwner()->GetComponent<Subject>()->Notify(*GetOwner(), hitEvent); // e.g. bullet notifies it's hitting
                //other->GetComponent<Subject>()->Notify(*other, hitEvent);          // e.g. player notifies it's hit

                // New overlap began
               // OnTriggerEnterEvent.Invoke(GetOwner(), other);
                
                //if (otherCollider)
                    //otherCollider->OnTriggerEnterEvent.Invoke(other, GetOwner());
            }
        }
    }

        // Check for overlaps that ended
        for (auto* previous : m_PreviousOverlaps)
        {
            if (!m_CurrentOverlaps.contains(previous) && GetOwner() < previous)
            {
                //std::cout << "OVERLAP ENDED" << std::endl;


                // Overlap ended
               // OnTriggerExitEvent.Invoke(GetOwner(), previous);
                //if (auto* otherCollider = previous->GetComponent<ColliderComponent>())
                    //otherCollider->OnTriggerExitEvent.Invoke(previous, GetOwner());
            }
        }

        // Save current as previous for next frame
        m_PreviousOverlaps = m_CurrentOverlaps;

	
}

//auto overlapPair = std::minmax(GetGameObject(), other);
//
//if (!m_CurrentOverlaps.contains(other) && !active_overlaps.contains(overlapPair))
//{
//    OnTriggerEnterEvent.Invoke(GetGameObject(), other);
//    if (const auto otherColliderComponent = other->GetComponent<ColliderComponent>())
//        otherColliderComponent->OnTriggerEnterEvent.Invoke(other, GetGameObject());
//
//    active_overlaps.insert(overlapPair);


bool ColliderComponent::IsOverlapping(const ColliderComponent* other)
{
    if (!other) return false;

    // Get position of both colliders
    const glm::vec3 posA = GetOwner()->GetTransform().GetWorldPosition();
    const glm::vec3 posB = other->GetOwner()->GetTransform().GetWorldPosition();

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