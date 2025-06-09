#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include <thread>

//#define  USE_STEAMWORKS
#ifdef USE_STEAMWORKS
#include <steam_api.h>
#endif

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}
//
//const glm::vec2 dae::Minigin::GetWindowSize() const
//{
//	int width = 0, height = 0;
//
//	if (g_window)
//	{
//		SDL_GetWindowSize(g_window, &width, &height);
//	}
//	else
//	{
//		std::cerr << "Warning: g_window is null. Returning (0,0) size.\n";
//	}
//
//	return glm::vec2(width, height);
//}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Minigin - Revekka Andronikidu",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowWidth,
		m_WindowHeight,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(/*const std::function<void()>& load*/)
{
	//load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = TimeManager::GetInstance();

	const int ms_per_frame = 1000 / m_fixedUpdateFramerate; //miliseconds pre frame
	float lag = 0.0f;
	
	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{
		time.Update();
		lag += time.GetDeltaTime();
		doContinue = input.ProcessInput();

		while (lag >= m_fixed_time_step)
		{
			sceneManager.FixedUpdate(m_fixed_time_step); //for physiscs and networking
			lag -= m_fixed_time_step;
		}		

		sceneManager.Update();
		renderer.Render();

#ifdef USE_STEAMWORKS
		SteamAPI_RunCallbacks();
#endif

		const auto sleep_time = time.GetLastFrameTime() + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);
	}
}


