#pragma once
#include "BaseComponent.h"
#include "Helpers.h"
#include <vector>
#include <bitset>

namespace dae
{
    enum class CellObject : uint8_t {
        PLATFORM = 0,  // Walkable surface
        LADDER = 1,  // Climbable ladder
        OFFSETLADDER = 2, //Offset Ladder (half cell)
        TRAY = 3,
        // Add more types as needed
    };

    class GridCell {
    public:
        void AddObject(CellObject type) { m_objects.set(static_cast<size_t>(type)); };
        void RemoveObject(CellObject type) { m_objects.reset(static_cast<size_t>(type)); };
        bool HasObject(CellObject type) const { return m_objects.test(static_cast<size_t>(type)); };
       // bool IsEmpty() const;

    private:
        std::bitset<8> m_objects;  // Supports up to 8 object types
    };

    struct CellPos
    {
        int x;
        int y;

        bool operator==(const CellPos& other) const
        {
            return x == other.x && y == other.y;
        }
    };


    struct CellPosHash 
    {
        std::size_t operator()(const CellPos& pos) const noexcept {
            return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
        }
    };

    class GridComponent : public dae::BaseComponent 
    {
    public:
      

        GridComponent(dae::GameObject* owner, int width, int height);
        ~GridComponent() = default;

        void Update() override {};
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        GridCell& GetCell(int x, int y);
        const GridCell& GetCell(int x, int y) const;
        bool IsValidPosition(int x, int y) const;
        void AddObjectToCell(int x, int y, CellObject type);
        void RemoveObjectFromCell(int x, int y, CellObject type);
        const GridCell& PositionToCell(int x, int y) const;

        bool IsOnPlatform(int posX, int posY) const;
        bool CanClimb(Direction dir, int posX, int posY) const;

        float GetLadderCenterX(int posX, int posY) const;

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        std::vector<CellPos> FindShortestPath(CellPos start, CellPos goal) const;

        std::vector<glm::vec3> ConvertPathToWorldPositions(const std::vector<CellPos>& path) const;
        CellPos WorldToCellPos(const glm::vec3& worldPos) const;

    private:
        int m_Width;
        int m_Height;
		int m_CellSize{ static_cast<int>(GameSettings::cellSize * GameSettings::scale.x) };
        std::vector<std::vector<GridCell>> m_grid;

        const int GetBaseCellX(int cellX) const;
        std::vector<CellPos> GetNeighbors(const CellPos& cell) const;
    };
}