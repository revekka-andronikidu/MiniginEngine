#pragma once
#include <glm.hpp>
#include <string>

namespace dae
{
	namespace GameSettings
	{
		inline constexpr  int cellSize{ 16 };
		inline constexpr  int HUDSize{ cellSize * 2};
		inline constexpr  int xOffset{ cellSize };
		inline constexpr  int windowHeight{ 208 + HUDSize };
		inline constexpr  int windowWidth{ 208 + xOffset };
		inline constexpr  glm::vec3 scale{ 3.f, 3.f, 3.f };
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
		Solo,
		Versus,
		Coop,
		Unknown
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

	enum class EnemyType
	{
		MrHotDog = 0,
		MrPickle = 1,
		MrEgg = 2
	};


	struct Sound
	{
		int id;
		const char* filename;

		// Constructor
		constexpr Sound(int i, const char* f) : id(i), filename(f) {}
	};

	namespace SoundID
	{
		inline constexpr Sound BGM{ 0, "BGM.wav" };
		inline constexpr Sound BurgerFall{ 1, "BurgerFall.wav" };
		inline constexpr Sound BurgerLand{ 2, "BurgerLand.wav" };
		inline constexpr Sound BurgerStep{ 3, "BurgerStep.wav" };
		inline constexpr Sound RoundClear{ 4, "RoundClear.wav" };
		inline constexpr Sound GameStart{ 5, "GameStart.wav" };
		inline constexpr Sound SystemSound{ 6, "SystemSound.wav" };
		inline constexpr Sound Death{ 7, "Death.wav" };
		inline constexpr Sound PepperShake{ 8, "PepperShake.wav" };
		inline constexpr Sound EnemyPeppered{ 9, "EnemySprayed.wav" };
		inline constexpr Sound EnemySqiushed{ 10, "EnemySquahed.wav" };
	}
	

	namespace SceneNames
	{
		const std::string MainMenu{ "Main Menu" };
		const std::string HighScores{ "High Scores" };
		const std::string HighScoreEntry{ "High Score Entry" };
		const std::string DummyScene{ "Empty" };
		const std::string GameOver{ "GameOver" };

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
		const std::string PLAYER_FEET{ "playerFeet" };
		const std::string ENEMY{ "enemy" };
		const std::string BURGER{ "burger" };
		const std::string TRAY{ "tray" };
		const std::string PLATFORM{ "platform" };
		const std::string LEVEL{ "level" };
		const std::string PEPPER{ "pepper" };
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
