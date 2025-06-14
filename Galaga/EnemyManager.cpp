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
   // LoadWavesFromFile("waves.txt");
    //StartWave(m_WaveNumber);

    auto game = GameManager::GetInstance().GetActiveGame();
    auto galaga = dynamic_cast<GalagaGame*>(game);

     m_WindowHeight = galaga->m_GameWidnowHeight;
     m_WindowWidth = galaga->m_GameWindowWidth;

     LoadPathsFromFile("paths.txt");
     LoadFormationFromFile("formations.txt");


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

    // Handle enemy spawning
    if (m_Spawning)
    {
        if (m_CurrentGroup < m_SpawnQueue.size())
        {
            m_SpawnTimer += TimeManager::GetInstance().GetDeltaTime();

            while (m_CurrentGroup < m_SpawnQueue.size() &&
                m_SpawnQueue[m_CurrentGroup].spawnTime <= m_SpawnTimer)
            {
                const auto& scheduled = m_SpawnQueue[m_CurrentGroup];
                SpawnEnemy(scheduled.info);
                ++m_CurrentGroup;
            }
        }
    }

}

void EnemyManager::StartStage(int stage)
{
    
    m_Spawning = true;
    m_CurrentStage = stage;
    StartWaveFromFormation(m_Formation);

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


void EnemyManager::SpawnEnemy(const FormationEnemyInfo& info)
{
    std::unique_ptr<GameObject> enemy;

    if (info.type == "Bee")
        enemy = ObjectFactory::GetInstance().CreateBee(glm::vec3{ info.position, 0 }, GetPathByName(info.pathName));
    else if (info.type == "Butterfly")
        enemy = ObjectFactory::GetInstance().CreateButterfly(glm::vec3{ info.position, 0 }, GetPathByName(info.pathName));
    else if (info.type == "BossGalaga")
        enemy = ObjectFactory::GetInstance().CreateBossGalaga(glm::vec3{ info.position, 0 }, GetPathByName(info.pathName));
    else
    {
        std::cerr << "Unknown enemy type: " << info.type << std::endl;
        return;
    }

    if (enemy)
        SceneManager::GetInstance().GetActiveScene().Add(std::move(enemy));
}

void EnemyManager::StartWaveFromFormation(std::vector<FormationEnemyInfo> formation)
{
    // Sort by group and subGroup
    std::ranges::sort(formation, [](const FormationEnemyInfo& a, const FormationEnemyInfo& b) {
        if (a.group == b.group)
            return a.subGroup < b.subGroup;
        return a.group < b.group;
        });

    m_SpawnQueue.clear();
    m_SpawnTimer = 0.0f;
    m_CurrentGroup = 0;

    float groupDelay = 0.0f;
    const float groupGap = 3.0f;     // seconds between groups
    const float subGroupGap = 0.1f;  // slight delay between subgroups

    int lastGroup = -1;
    int lastSubGroup = -1;

    for (const auto& info : formation)
    {
        if (info.group != lastGroup)
        {
            groupDelay += groupGap;
            lastGroup = info.group;
            lastSubGroup = -1;
        }

        if (info.subGroup != lastSubGroup)
        {
            groupDelay += subGroupGap;
            lastSubGroup = info.subGroup;
        }

        m_SpawnQueue.push_back(ScheduledSpawn{ groupDelay, info });
    }
}

std::vector<glm::vec2> EnemyManager::GetPathByName(const std::string& name) const
{
    for (const auto& path : m_Paths)
    {
        if (path.name == name)
        {
            return path.points;
        }
    }

    std::cerr << "Path not found: " << name << std::endl;
    return {}; // Return empty if not found
}

void EnemyManager::LoadFormationFromFile(const std::string& filename)
{
    std::ifstream file("../Data/" + filename);

    if (!file.is_open())
    {
        std::cerr << "Failed to open formation file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string token;

        std::vector<std::string> tokens;
        while (std::getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        if (tokens.size() != 6)
        {
            std::cerr << "Malformed line in formation file: " << line << std::endl;
            continue;
        }

        std::string type = tokens[0];
        int col = std::stoi(tokens[1]);
        int row = std::stoi(tokens[2]);
        int group = std::stoi(tokens[3]);
        int subGroup = std::stoi(tokens[4]);
        std::string pathName = tokens[5];

        glm::vec2 worldPos = GridToWorldPosition(col, row);
        m_Formation.emplace_back(FormationEnemyInfo{ type, worldPos, group, subGroup, pathName });
    }
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



