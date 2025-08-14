#pragma once
#include "json.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <Singleton.h>
namespace dae
{
    struct HighScore {
        std::string name;
        int score;
    };

    class HighScoreManager final : public Singleton<HighScoreManager>
    {
    public:
           void Load() 
        {
            std::ifstream file(m_FilePath);
            if (!file.is_open()) {
                // If file doesn't exist, start with empty scores
                m_Scores.clear();
                return;
            }

            nlohmann::json j;
            file >> j;

            m_Scores.clear();
            for (auto& entry : j["highscores"]) {
                m_Scores.push_back({ entry["name"], entry["score"] });
            }
        }

        void Save() const {
            nlohmann::json j;
            j["highscores"] = nlohmann::json::array();
            for (const auto& score : m_Scores) {
                j["highscores"].push_back({ {"name", score.name}, {"score", score.score} });
            }
            std::ofstream file(m_FilePath);
            file << j.dump(4); // pretty print
        }

        void AddScore(const std::string& name, int score) {
            m_Scores.push_back({ name, score });
            std::sort(m_Scores.begin(), m_Scores.end(), [](const HighScore& a, const HighScore& b) {
                return b.score < a.score; // descending order
                });
            if (m_Scores.size() > 5) {
                m_Scores.resize(5); // keep only top 5
            }
            Save();
        }

        const std::vector<HighScore>& GetScores() const { return m_Scores; }
        const int GetHighestScore() const 
		{
            int maxScore = 0;
            for (const auto& score : m_Scores)
            {
                if (score.score > maxScore)
                    maxScore = score.score;
            }
            return maxScore;
        }
        void Init(const std::string& filename) 
        {
            const auto path = dae::ResourceManager::GetInstance().GetDataPath();
            m_FilePath = path / filename;
            Load();
        }


    private:
        friend class Singleton<HighScoreManager>;
        HighScoreManager()
        {
            auto file{ "highscores.json" };
            const auto path = dae::ResourceManager::GetInstance().GetDataPath();
            m_FilePath = path / file;
            Load();
        }
		std::filesystem::path m_FilePath{};
        std::vector<HighScore> m_Scores;
    };

}