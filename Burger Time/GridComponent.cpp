// GridComponent.cpp
#include "GridComponent.h"
#include <set>
#include <queue>
#include <unordered_map>

using namespace dae;

GridComponent::GridComponent(dae::GameObject* owner, int width, int height)
    : BaseComponent(owner), m_Width(width), m_Height(height) {
    m_grid.resize(m_Height);
    for (auto& row : m_grid) {
        row.resize(m_Width);
    }
}
GridCell& GridComponent::GetCell(int x, int y) {
    return m_grid[y][x];
}

const GridCell& GridComponent::GetCell(int x, int y) const {
    return m_grid[y][x];
}

bool GridComponent::IsValidPosition(int x, int y) const {
    return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
}

void GridComponent::AddObjectToCell(int x, int y, CellObject type) {
    if (IsValidPosition(x, y)) {
        m_grid[y][x].AddObject(type);
    }
}

void GridComponent::RemoveObjectFromCell(int x, int y, CellObject type) {
    if (IsValidPosition(x, y)) {
        m_grid[y][x].RemoveObject(type);
    }
}

const GridCell& GridComponent::PositionToCell(int x, int y) const
{
        //int cellSize = GameSettings::cellSize * GameSettings::scale.x ;
        int cellX = x / m_CellSize;
        int cellY = y / m_CellSize;

        if (IsValidPosition(cellX, cellY)) {
            return m_grid[cellY][cellX];
        }

        GridCell dummyCell{};
        return dummyCell;
}

std::vector<glm::vec3> GridComponent::ConvertPathToWorldPositions(const std::vector<CellPos>& path) const
{
    std::vector<glm::vec3> worldPath;
    int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);

    for (const auto& cell : path)
    {
        const GridCell& gridCell = GetCell(cell.x, cell.y);

        float x = cell.x*cellSize + cellSize / 2.0f; // default: center of cell
        float y = cell.y*cellSize + cellSize / 2.0f; // centerY

        // Adjust X if the cell has an offset ladder
        if (gridCell.HasObject(CellObject::OFFSETLADDER))
        {
           x += cellSize / 2.0f; // shift right for offset ladder
        }

		//x += cellSize / 2; // Center the X position in the cell
		//y += cellSize / 2; // Center the Y position in the cell


        worldPath.emplace_back(x, y, 0.0f); // z = 0
    }

    return worldPath;
}


bool GridComponent::IsOnPlatform(int posX, int posY) const
{
    //const int cellSize = GameSettings::cellSize * GameSettings::scale.x; // 48 px
    int cellY = posY / m_CellSize;
    int localY = m_CellSize -( posY % m_CellSize); // Y position within the cell (0 = bottom)

    // Only true if in bottom third (0 to 15 px of the cell)
    if (localY <= m_CellSize / 3)
    {
        int cellX = posX / m_CellSize;
        if (IsValidPosition(cellX, cellY))
        {
            const auto& cell = m_grid[cellY][cellX];
            if (cell.HasObject(CellObject::PLATFORM))
            {
                return true;
            }
        }
    }
    return false;
}

bool GridComponent::CanClimb(Direction dir, int posX, int posY) const
{
    //const int cellSize = GameSettings::cellSize * GameSettings::scale.x;
    const int cellX = posX / m_CellSize;
    const int cellY = posY / m_CellSize;
    const int localX = posX % m_CellSize;
    const int localY = m_CellSize - (posY % m_CellSize);

    const int margin = 4* GameSettings::scale.x; //4px times scale


    if (!IsValidPosition(cellX, cellY))
        return false;

    const auto& currentCell = GetCell(cellX, cellY);
    bool isOnNormalLadder = currentCell.HasObject(CellObject::LADDER);

    bool isOnOffsetLadderRight{ false };
    if (IsValidPosition(cellX - 1, cellY))
    {
        const auto& currentLeftCell = GetCell(cellX - 1, cellY);
        isOnOffsetLadderRight = currentLeftCell.HasObject(CellObject::OFFSETLADDER);
    }

    bool isOnOffsetLadderLeft = currentCell.HasObject(CellObject::OFFSETLADDER);
    bool isOnOffsetLadder = isOnOffsetLadderRight || isOnOffsetLadderLeft;

    bool isOnLadder = isOnNormalLadder || isOnOffsetLadder;
    
    // Helper lambda to check for any ladder below

    bool offsetLadderBelowLeft = false;
    bool offsetLadderBelowCenter = false;


 auto hasLadderBelow = [&]() -> bool
     {
         bool ladder{ false };
         
             if (!IsValidPosition(cellX, cellY + 1))
                 return false;

             const auto& belowCell = GetCell(cellX, cellY + 1);
             ladder = belowCell.HasObject(CellObject::LADDER);
             offsetLadderBelowCenter = belowCell.HasObject(CellObject::OFFSETLADDER);
                    
             if (IsValidPosition(cellX - 1, cellY + 1))
             {
                 const auto& belowLeftCell = GetCell(cellX - 1, cellY + 1);
                 offsetLadderBelowLeft = belowLeftCell.HasObject(CellObject::OFFSETLADDER);
             }

         return ladder || offsetLadderBelowCenter || offsetLadderBelowLeft;
     };


#pragma region MarginCheck
 // --- Position Margin Constraints ---
 bool ladderBelow = hasLadderBelow();

 if (isOnLadder || ladderBelow) {
     bool positionValid = true;

     if (isOnNormalLadder || ladderBelow && !offsetLadderBelowLeft && !offsetLadderBelowCenter) {
         // Normal ladder in current or below
         positionValid = localX >= margin && localX <= (m_CellSize - margin);
     }
     else if (isOnOffsetLadderRight || offsetLadderBelowLeft) {
         // Offset ladder is in left cell
         positionValid = localX <= (m_CellSize / 2 - margin);
     }
     else if (isOnOffsetLadderLeft || offsetLadderBelowCenter) {
         // Offset ladder in current or center below
         positionValid = localX >= (m_CellSize / 2 + margin);
     }

     if (!positionValid)
         return false;
 }
#pragma endregion


    if (dir == Direction::Up) {
        return isOnLadder || (hasLadderBelow() && localY < 3 * GameSettings::scale.x);
    }

    if (dir == Direction::Down) {
        bool ladderBelow = hasLadderBelow();
        return (isOnLadder && (ladderBelow || localY > 3 * GameSettings::scale.y)) || ladderBelow;
    }
}

float GridComponent::GetLadderCenterX(int posX, int posY) const
{
    //const int cellSize = GameSettings::cellSize * GameSettings::scale.x;
    int cellX = posX / m_CellSize;
    int cellY = posY / m_CellSize;

    if (!IsValidPosition(cellX, cellY))
        return 0;

    const std::set<int> offsetLadderCells = { 1, 2, 4, 5, 7, 8, 10, 11 };

    if (offsetLadderCells.contains(cellX))
    {
        int baseCellX = GetBaseCellX(cellX);
        return (baseCellX + 0.5f) * m_CellSize;
    }
    else
    {
        return (cellX) * m_CellSize;
    }
}

// Given a cellX, find the base cell of the pair (lower cell of the two cells).
const int GridComponent::GetBaseCellX(int cellX) const
{
    // List of pairs start cells
    std::vector<int> pairStarts = { 1, 4, 7, 10 };

    for (int start : pairStarts)
    {
        if (cellX == start || cellX == start + 1)
            return start;
    }
    // If no pair found, return cellX itself (single cell)
    return cellX;
}

std::vector<CellPos> GridComponent::FindShortestPath(CellPos start, CellPos goal) const
{
    if (!IsValidPosition(start.x, start.y) || !IsValidPosition(goal.x, goal.y))
        return {};

    // BFS frontier (cells to explore)
    std::queue<CellPos> frontier;
    frontier.push(start);

    // "Came from" map to reconstruct path later
    std::unordered_map<CellPos, CellPos, CellPosHash> cameFrom;
    cameFrom[start] = start;

    // BFS loop
    while (!frontier.empty()) {
        CellPos current = frontier.front();
        frontier.pop();

        if (current == goal)
            break;

        // Explore valid neighbors
        for (const auto& neighbor : GetNeighbors(current)) {
            if (cameFrom.find(neighbor) == cameFrom.end()) {
                frontier.push(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }

    // No path found
    if (cameFrom.find(goal) == cameFrom.end())
        return {};

    std::vector<CellPos> path;
    for (CellPos at = goal; !(at == start); at = cameFrom[at]) {
        path.push_back(at);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;

}

std::vector<CellPos> GridComponent::GetNeighbors(const CellPos& cell) const
{
    std::vector<CellPos> neighbors;
    int x = cell.x;
    int y = cell.y;

    // Left
    if (IsValidPosition(x - 1, y) && GetCell(x - 1, y).HasObject(CellObject::PLATFORM))
        neighbors.push_back({ x - 1, y });

    // Right
    if (IsValidPosition(x + 1, y) && GetCell(x + 1, y).HasObject(CellObject::PLATFORM))
        neighbors.push_back({ x + 1, y });

    // Up (ladder)
    if (IsValidPosition(x, y - 1) && GetCell(x, y).HasObject(CellObject::LADDER))
        neighbors.push_back({ x, y - 1 });

    // Down (ladder)
    if (IsValidPosition(x, y + 1) && GetCell(x, y + 1).HasObject(CellObject::LADDER))
        neighbors.push_back({ x, y + 1 });

    // Up (ladder)
    if (IsValidPosition(x, y - 1) && GetCell(x, y).HasObject(CellObject::OFFSETLADDER))
        neighbors.push_back({ x, y - 1 });

    // Down (ladder)
    if (IsValidPosition(x, y + 1) && GetCell(x, y + 1).HasObject(CellObject::OFFSETLADDER))
        neighbors.push_back({ x, y + 1 });

    return neighbors;
}

CellPos GridComponent::WorldToCellPos(const glm::vec3& worldPos) const
{
    int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);

    int cellX = static_cast<int>(worldPos.x) / cellSize;
    int cellY = static_cast<int>(worldPos.y) / cellSize;

    return { cellX, cellY };
}