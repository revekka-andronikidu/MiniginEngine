#pragma once

namespace dae
{
	class EnemyManager final 
	{

	public:
		EnemyManager() = default;
		virtual ~EnemyManager() {}
		EnemyManager(const EnemyManager& other) = delete;
		EnemyManager(EnemyManager&& other) = delete;
		EnemyManager& operator=(const EnemyManager& other) = delete;
		EnemyManager& operator=(EnemyManager&& other) = delete;

	private:


		
	};

	
}

