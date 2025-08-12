#pragma once
#include "Singleton.h"
#include "GameState.h"

namespace dae
{
	class BurgerTimeGame;
	class SceneFactory final : public Singleton<SceneFactory>
	{
	private:
		friend class Singleton<SceneFactory>;
		SceneFactory();

		BurgerTimeGame* m_Game;

	public:
		void CreateMainMenu();
		void CreateLevel(unsigned short stage);
		void CreateHUD();
	};

}
