#pragma once
#include "BaseComponent.h"
#include <vec3.hpp>
#include <unordered_set>
#include "Subject.h"

namespace dae
{
    struct ColliderShape {
        glm::vec3 size;
        glm::vec3 offset;
        int mask; // Bitmask or category
    };

	class ColliderComponent : public BaseComponent, public Subject
	{
    public:
        ColliderComponent(GameObject* owner , const glm::vec3 size, const glm::vec3 offset = {0,0,0}) : BaseComponent(owner)
            , m_Size{size}
            , m_Offset{offset}
            , m_IsActive{ true }
        {
        };

        virtual ~ColliderComponent() = default;

        void AddShape(const glm::vec3& size, const glm::vec3& offset, int mask) 
        {
            m_Shapes.push_back({ size, offset, mask });
        }

        //const std::vector<ColliderShape>& GetShapes() const { return m_Shapes; }
       
        void Update() override { UpdateOverlaps(); };
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};
        
        bool m_IsActive{};

    private:
        const glm::vec3 m_Size;
        const glm::vec3 m_Offset;
        std::unordered_set<GameObject*> m_CurrentOverlaps{};
        std::unordered_set<GameObject*> m_PreviousOverlaps{};
        std::vector<ColliderShape> m_Shapes;

        void UpdateOverlaps();
        bool IsOverlapping(const ColliderComponent* other);
	};

}

