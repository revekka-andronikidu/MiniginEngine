#pragma once
#include <string>
#include <functional>
#include "TimeManager.h"


namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath, int windowWidth, int windowHeight);
		~Minigin();
		void Run();
		const int GetWindowWidth() const { return m_WindowWidth; } ;
		const int GetWindowHeight() const { return m_WindowHeight; };

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		const int m_WindowWidth = 416;
		const int m_WindowHeight = 416;

	private:
		const int m_fixedUpdateFramerate{TimeManager::GetInstance().GetFixedFrameRate()};
		const float m_fixed_time_step{ 1.f / m_fixedUpdateFramerate };
		
	};
}