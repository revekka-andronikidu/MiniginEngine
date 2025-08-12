#pragma once
#include <glm.hpp>
#include <string>

namespace dae
{
	namespace GameSettings
	{
		inline  int windowWidth{208};
		inline  int cellSize{ 16 };
		inline  int HUDSize{ cellSize * 2};
		inline  int windowHeight{ 208 + HUDSize };
		inline  glm::vec3 scale{ 3.f, 3.f, 3.f };
	}

	enum class Direction
	{
		None,
		Up,
		Down,
		Left,
		Right
	};

	enum class GameMode
	{
		None,
		Solo,
		Versus,
		Coop
	};

	enum class IngredientType
	{
		BunTop = 0,
		BunBottom = 1,
		Cheese = 2,
		Patty = 3,
		Tomato = 4,
		Lettuce = 5
	};

	namespace SceneNames
	{
		const std::string MainMenu{ "Main Menu" };
		const std::string HighScores{ "High Scores" };

		const std::string Stage1{ "Stage1" };
		const std::string Stage2{ "Stage2" };
		const std::string Stage3{ "Stage3" };
		
		inline std::string GetStageName(int stage)
		{
			return "Stage" + std::to_string(stage);
		}
	}

	namespace Tag
	{
		const std::string PLAYER{ "player" };
		const std::string ENEMY{ "enemy" };
		const std::string BURGER{ "burger" };
		const std::string TRAY{ "tray" };
		const std::string PLATFORM{ "platform" };
	}


	inline glm::vec3 DirectionToVector(Direction dir)
	{
		switch (dir)
		{
		case Direction::Up:    return glm::vec3(0.f, -1.f, 0.f);
		case Direction::Down:  return glm::vec3(0.f, 1.f, 0.f);
		case Direction::Left:  return glm::vec3(-1.f, 0.f, 0.f);
		case Direction::Right: return glm::vec3(1.f, 0.f, 0.f);
		default:               return glm::vec3(0.f); // Direction::None
		}
	}
}
