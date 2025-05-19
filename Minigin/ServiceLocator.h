#pragma once
#include "Audio.h"


namespace dae
{

	class ServiceLocator final
	{
	public:
		ServiceLocator() = default;
		~ServiceLocator() = default;

		ServiceLocator(const ServiceLocator& other) = delete;
		ServiceLocator(ServiceLocator&& other) = delete;
		ServiceLocator& operator=(const ServiceLocator& other) = delete;
		ServiceLocator& operator=(ServiceLocator&& other) = delete;
		

		//static void Initialize();

		static void RegisterAudioService(std::unique_ptr<IAudio> pService) {	m_pAudioService = pService == nullptr ? std::make_unique<NullAudio>() : std::move(pService); };
		static IAudio& GetAudioService() { return *m_pAudioService; };


	private:
		static std::unique_ptr<IAudio> m_pAudioService;
	};
}


