#pragma once
#include "Singleton.h"

namespace dae
{
	class GameManager final : public Singleton<GameManager>
	{
	public:


	private:
		friend class Singleton<GameManager>;
		GameManager() = default;

	};

};

