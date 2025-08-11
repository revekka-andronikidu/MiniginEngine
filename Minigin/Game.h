#pragma once
#include "GameManager.h"
#include "glm.hpp"

namespace dae
{
	struct GameInitParams 
	{
		int windowWidth{};
		int windowHeight{};
		glm::vec3 scale{};
		int cellSize{};
		int HUDSize{};
		//sprite size
		//cell size
	};

	class Game 
	{
		template <typename T, typename std::enable_if<std::is_base_of<Game, T>::value>::type*>
		friend Game& GameManager::CreateGame();
		
	public:
		//virtual void Update(float elapsed) = 0;

		Game() = default;
		virtual ~Game() = default;
		virtual void Update() = 0;

		virtual void Initialize(const GameInitParams& gameInitParms) = 0;

		Game(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator= (const Game&) = delete;
		Game& operator= (const Game&&) = delete;

		//virtual bool IsGameRunning() = 0;
	};

}



