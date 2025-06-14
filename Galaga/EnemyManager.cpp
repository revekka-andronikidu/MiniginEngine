#include "EnemyManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "ObjectFactory.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace dae;

EnemyManager::EnemyManager()
{
    LoadPathsFromFile("paths.txt");
   // LoadWavesFromFile("waves.txt");
    //StartWave(m_WaveNumber);
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

