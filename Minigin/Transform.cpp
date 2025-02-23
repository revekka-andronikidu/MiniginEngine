#include "Transform.h"
#include "GameObject.h"



void dae::Transform::SetDirty() 
{
    if (!m_IsDirty) 
    {
        m_IsDirty = true;

        auto children = m_pOwner->GetChildren();

        for (auto child : children) {
            child->GetTransform().SetDirty();
        }
    }
}

void dae::Transform::SetPosition(const glm::vec3& newPosition)
{
    m_LocalPosition = newPosition;
    UpdateLocalMatrix();
    SetDirty();
}

void dae::Transform::SetRotation(const glm::quat& newRotation)
{
    m_LocalRotation = newRotation;
    UpdateLocalMatrix();
    SetDirty();
};

void dae::Transform::SetScale(const glm::vec3& newScale)
{
    m_LocalScale = newScale;
    UpdateLocalMatrix();
    SetDirty();
}

void dae::Transform::UpdateLocalMatrix()
{
    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_LocalPosition);
    const glm::mat4 rotationMat = glm::mat4_cast(m_LocalRotation);
    const glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), m_LocalScale);
    m_LocalMatrix = translation * rotationMat * scaleMat;
}

void dae::Transform::SetLocalMatrix(const glm::mat4& matrix)
{
    DecomposeMatrix(matrix, m_LocalPosition, m_LocalRotation, m_LocalScale);
    UpdateLocalMatrix();
    SetDirty();
}

void dae::Transform::DecomposeMatrix(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale) 
{
    // Extract translation
    translation = glm::vec3(transform[3]);

    // Extract scale
    glm::vec3 xAxis = glm::vec3(transform[0]);
    glm::vec3 yAxis = glm::vec3(transform[1]);
    glm::vec3 zAxis = glm::vec3(transform[2]);

    scale.x = glm::length(xAxis);
    scale.y = glm::length(yAxis);
    scale.z = glm::length(zAxis);

    // Remove scaling from matrix axes
    xAxis = glm::normalize(xAxis);
    yAxis = glm::normalize(yAxis);
    zAxis = glm::normalize(zAxis);

    // Reconstruct rotation matrix and convert to quaternion
    const glm::mat3 rotMatrix(
        xAxis.x, yAxis.x, zAxis.x,
        xAxis.y, yAxis.y, zAxis.y,
        xAxis.z, yAxis.z, zAxis.z
    );

    rotation = glm::quat_cast(rotMatrix);
}

const glm::mat4& dae::Transform::GetWorldMatrix() const
{
    if (m_IsDirty) 
    {
        UpdateWorldMatrix();
    }
    return m_WorldMatrix;
}

const glm::vec3& dae::Transform::GetWorldPosition() const
{
    if (m_IsDirty)
    {
        UpdateWorldPosition();
    }
    return m_WorldPosition;
}


void dae::Transform::UpdateWorldPosition() const
{
    if (m_IsDirty)
    {
        auto parent = m_pOwner->GetParent();
        if (parent == nullptr)
        {
            m_WorldPosition = m_LocalPosition;
        }
        else
        {
            m_WorldPosition = parent->GetTransform().GetWorldPosition() + m_LocalPosition;
        }
    }
    m_IsDirty = false;
}

void dae::Transform::UpdateWorldMatrix() const
{
    if (!m_IsDirty)
        return;

    auto parent = m_pOwner->GetParent();
    if (parent == nullptr)
    {
        m_WorldMatrix = m_LocalMatrix;
    }
    else
    {
        m_WorldMatrix = parent->GetTransform().GetWorldMatrix() * m_LocalMatrix;
    }

    m_IsDirty = false;
}

