#pragma once
#include "Singleton.h"
#include <memory>

namespace dae
{
	class BurgerTimeGame;
	class Scene;
	class GridComponent;
	class GameObject;
	class SceneFactory final : public Singleton<SceneFactory>
	{
	public:
		void CreateMainMenu();
		void CreateHighScoreEntry();
		void CreateHighScores();
		void CreateLevel(unsigned short stage);
		void CreateGameOver();

	private:
		friend class Singleton<SceneFactory>;
		SceneFactory();

		BurgerTimeGame* m_Game;
		void CreateHUD(Scene& scene, GameObject* playerPtr);

		std::unique_ptr<GameObject> CreatePlayer(Scene& scene, int x, int y, GridComponent* grid);
		void CreateEnemies(Scene& scene);
	};

}
