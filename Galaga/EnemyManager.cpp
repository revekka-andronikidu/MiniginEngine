#include <algorithm>
#include <random>
#include <ranges>
#include "EnemyManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "ObjectFactory.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "EnemyAttackComponent.h"
#include "TimeManager.h" 
#include "GalagaGame.h"

using namespace dae;

EnemyManager::EnemyManager()
{
    LoadPathsFromFile("paths.txt");
    LoadFormationFromFile("formations.txt");
   // LoadWavesFromFile("waves.txt");
    //StartWave(m_WaveNumber);

    auto game = GameManager::GetInstance().GetActiveGame();
    auto galaga = dynamic_cast<GalagaGame*>(game);

     m_WindowHeight = galaga->m_GameWidnowHeight;
     m_WindowWidth = galaga->m_GameWindowWidth;
}

void EnemyManager::Update()
{
    if (!m_IsAttacking)
    {
        m_AttackTimmer += dae::TimeManager::GetInstance().GetDeltaTime();
        if (m_AttackTimmer >= m_AttackInterval)
        {
            StartAttack();
            m_IsAttacking = true;
        }
    }
}

void EnemyManager::StartStage(int stage)
{
   // if (stage >= static_cast<int>(m_Stages.size()))
       // return;

   // m_CanAttack = false;
    m_CurrentStage = stage;
    m_EnemiesInCurrentStage = 0;
    m_CurrentGroup = 0;
    //m_GroupQueues.clear();


    
    auto bee = ObjectFactory::GetInstance().CreateBee(glm::vec3{ 200, 80, 0 }, m_Paths[0].points);
    SceneManager::GetInstance().GetScene("Solo").Add(std::move(bee));


}

void EnemyManager::StartAttack()
{
    const auto enemies = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag("enemy");
    const auto attackers = SelectAttackers(enemies);

    for (auto enemy : attackers)
    {
        if (const auto attackComponent = enemy->GetComponent<EnemyAttackComponent>())
        {
            attackComponent->StartAttack();
            m_Attackers.push_back(enemy);

        }
    }
}

std::vector<FormationEnemyInfo> EnemyManager::LoadFormationFromFile(const std::string& filename)
{
    std::vector<FormationEnemyInfo> formation;
    std::ifstream file("../Data/" + filename);

    if (!file.is_open())
    {
        std::cerr << "Failed to open formation file: " << filename << std::endl;
        return formation;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string type, pathName;
        int col, row, group, subGroup;
        char comma;

        ss >> type >> comma >> col >> comma >> row >> comma >> group >> comma >> subGroup >> comma >> pathName;

        glm::vec2 worldPos = GridToWorldPosition(col, row);
        formation.emplace_back(FormationEnemyInfo{ type, worldPos, group, subGroup, pathName });
    }

    return formation;
}

std::vector<dae::GameObject*> EnemyManager::SelectAttackers(const std::vector<dae::GameObject*>& enemies) const
{
    std::vector<dae::GameObject*> availableAttackers;

    for (const auto& enemy : enemies)
    {
        if (std::find(m_Attackers.begin(), m_Attackers.end(), enemy) == m_Attackers.end())
        {
            availableAttackers.push_back(enemy);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());

    const int numAttackers = std::min(static_cast<int>(availableAttackers.size()), m_MaxAttackersAtOnce);
    std::ranges::shuffle(availableAttackers, g);

    // Take first `numAttackers` elements after shuffle
    std::vector<dae::GameObject*> selectedAttackers{
        availableAttackers.begin(),
        availableAttackers.begin() + numAttackers
    };

    return availableAttackers;
}


void EnemyManager::LoadPathsFromFile(const std::string& filename)
{
    const std::filesystem::path filePath = std::filesystem::path("../Data/") / filename;

    try
    {
        if (!exists(filePath))
        {
            throw std::runtime_error("File does not exist: " + filePath.string());
        }

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open file: " + filePath.string());
        }

        std::string line;
        Path currentPath;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                if (!currentPath.name.empty())
                {
                    m_Paths.push_back(currentPath);
                    currentPath = Path{};
                }
                continue;
            }

            if (currentPath.name.empty())
            {
                currentPath.name = line;
            }
            else
            {
                float x, y;
                std::istringstream ss(line);
                ss >> x;
                ss.ignore(1, ',');
                ss >> y;
                currentPath.points.emplace_back(x, y);
            }
        }

        if (!currentPath.name.empty())
        {
            m_Paths.push_back(currentPath);
        }
        file.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }


}

glm::vec2 EnemyManager::GridToWorldPosition(int col, int row)
{
    const float totalWidth = m_WindowWidth - 2 * kHorizontalPadding;
    const float cellWidth = totalWidth / static_cast<float>(kGridCols);
    const float x = kHorizontalPadding + (col + 0.5f) * cellWidth;
    const float y = kTopOffset + row * kVerticalSpacing;

    return { x, y };
}



