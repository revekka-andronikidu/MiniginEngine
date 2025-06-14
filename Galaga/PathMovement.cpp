#include "PathMovement.h"
#include "GameObject.h"
#include "GalagaGame.h"
#include <TimeManager.h>

using namespace dae;


PathMovement::PathMovement(dae::GameObject* gameObject, const std::vector<glm::vec2>& path, float speed)
    : BaseComponent(gameObject)
    , m_Path{ path }
    , m_Speed{speed}
    , m_CurrentPoint{ 0 }
    , m_PathComplete{false}
{
}
 

void PathMovement::Update()
{
  if (!m_PathComplete)
    {
        MoveAlongPath();
    }
}

void PathMovement::MoveAlongPath()
{
  
    if (m_CurrentPoint >= m_Path.size())
    {
        
            m_PathComplete = true;
            //OnPathCompleted.Invoke();
            return;
        
    }

    auto position = GetOwner()->GetTransform().GetWorldPosition();
    const glm::vec2 target = ConvertToWorldCoordinates(m_Path[m_CurrentPoint]);

    glm::vec2 direction = target - glm::vec2(position.x, position.y);
    const float distance = length(direction);

    if (distance < m_Speed * TimeManager::GetInstance().GetDeltaTime())
    {
        GetOwner()->GetTransform().SetPosition(target.x, target.y, position.z);
        m_CurrentPoint++;
    }
    else
    {
        direction = normalize(direction);
        position.x += direction.x * m_Speed * TimeManager::GetInstance().GetDeltaTime();
        position.y += direction.y * m_Speed * TimeManager::GetInstance().GetDeltaTime();
        

        // Update position
        GetOwner()->GetTransform().SetPosition(position.x, position.y, position.z);
    }
}

glm::vec2 PathMovement::ConvertToWorldCoordinates(const glm::vec2& relativePos)
{
    auto game = GameManager::GetInstance().GetActiveGame();
    auto galaga = dynamic_cast<GalagaGame*>(game);

    float x = relativePos.x * galaga->m_GameWindowWidth;
    float y = relativePos.y * galaga->m_GameWidnowHeight;
    return { x, y };
}

void PathMovement::StartAtFirstPoint()
{
    if (!m_Path.empty())
    {
        m_CurrentPoint = 0;
        GetOwner()->GetTransform().SetPosition(ConvertToWorldCoordinates(m_Path[m_CurrentPoint]).x, ConvertToWorldCoordinates(m_Path[m_CurrentPoint]).y, GetOwner()->GetTransform().GetWorldPosition().z);
    }
}

void PathMovement::AddWorldSpacePoint(const glm::vec2& point)
{
    auto game = GameManager::GetInstance().GetActiveGame();
    auto galaga = dynamic_cast<GalagaGame*>(game);

    const float screenWidth = static_cast<float>(galaga->m_GameWindowWidth);
    const float screenHeight = static_cast<float>(galaga->m_GameWidnowHeight);
    glm::vec2 normalizedPoint = glm::vec2(point.x / screenWidth, point.y / screenHeight);
    m_Path.emplace_back(normalizedPoint);
}