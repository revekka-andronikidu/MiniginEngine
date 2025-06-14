#include "Audio.h"
#include <iostream>
#include "SDL.h"
#include "SDL_mixer.h"
#include <algorithm>
#include <stdexcept>
#include "SoundHandle.h"
#include "ResourceManager.h"
#include <vld.h>

namespace dae
{
	std::mutex SDLAudio::mutex;

	class SDLAudio::SDL_AudioImpl
	{
	public:
		SDL_AudioImpl()
		{
			/*if (SDL_Init(SDL_INIT_AUDIO) < 0)
			{
				throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
			}*/

			if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) //to enable MP3 format
			{
				Mix_Quit();  // Cleanup mixer before throwing
				throw std::runtime_error(std::string("Mix_Init Error: ") + Mix_GetError());
			}

			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
			{
				Mix_Quit();  // Cleanup mixer before throwing
				throw std::runtime_error(std::string("Mix_OpenAudio Error: ") + Mix_GetError());
			}
			VLDMarkAllLeaksAsReported();
		}

		~SDL_AudioImpl()
		{
			
			Mix_CloseAudio();
			Mix_Quit();
			SDL_QuitSubSystem(SDL_INIT_AUDIO);
			std::cout << "Destroying SDL_AudioImpl\n";
		}

		std::shared_ptr<ISoundHandle>LoadSound(const std::string& file)
		{
			Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
			if (!chunk) throw std::runtime_error("Failed to load sound: " + file);

			// Wrap in shared_ptr with correct deleter
			std::shared_ptr<Mix_Chunk> sound(chunk, Mix_FreeChunk);

			// Wrap in your SoundHandle
			return std::make_shared<SoundHandle<Mix_Chunk>>(sound);


			// Wrap Mix_Chunk in SoundHandle<Mix_Chunk>
			//auto sound = std::make_shared<Mix_Chunk>(chunk, Mix_FreeChunk);

			//std::shared_ptr<ISoundHandle> handle = std::make_shared<SoundHandle<Mix_Chunk>>(sound);
		}

		void PlaySound(sound_id soundId, const float volume, bool looping)
		{
			std::lock_guard<std::mutex> lock(mutex);

			// Add sound request to queue
			SoundRequest request
			{
				ResourceManager::GetInstance().GetSound(soundId),
				std::clamp(volume, 0.0f, 1.0f),
				looping
			};
			m_SoundQueue.push(request);

			// Process as much of the queue as possible
			ProcessQueue();
		}

		/*void PlayMusic(sound_id soundId, const float volume, bool looping)
		{
			
		}

		void SetMusicVolume(const float volume)
		{
			
		}
		void SetSoundVolume(const float volume)
		{
			
		}*/

		void SetMasterVolume(const float volume)
		{
			std::lock_guard<std::mutex> lock(mutex);

			m_volume = std::clamp(volume, 0.0f, 1.0f);
			Mix_MasterVolume(static_cast<int>(m_volume * MIX_MAX_VOLUME));
		}

		void StopAllSounds()
		{
			std::lock_guard<std::mutex> lock(mutex);
			Mix_HaltChannel(-1);


		}

		void Mute()
		{
			std::lock_guard<std::mutex> lock(mutex);
			
				m_Muted = !m_Muted;
				Mix_MasterVolume(static_cast<int>( m_Muted * MIX_MAX_VOLUME));

		}

		void StopSound(sound_id soundId)
		{
			std::lock_guard<std::mutex> lock(mutex);

			auto baseHandle = ResourceManager::GetInstance().GetSound(soundId);
			auto target = GetChunk(baseHandle);

			// Stop playing channels
			int numChannels = Mix_AllocateChannels(-1);
			for (int i = 0; i < numChannels; ++i)
			{
				if (Mix_GetChunk(i) == target)
				{
					Mix_HaltChannel(i);
				}
			}

			// Remove from queue
			RemoveFromQueue(target);
			
		}

	private:

		struct SoundRequest
		{
			std::shared_ptr<ISoundHandle> chunk;
			float volume;
			bool looping;
		};

		Mix_Chunk* GetChunk(std::shared_ptr<ISoundHandle> handle)
		{
			
			// Try to cast to SoundHandle<Mix_Chunk>
			auto typedHandle = std::dynamic_pointer_cast<SoundHandle<Mix_Chunk>>(handle);
			if (!typedHandle)
			{
				throw std::runtime_error("StopSound: Sound ID is not a Mix_Chunk type.");
			}

			std::shared_ptr<Mix_Chunk> chunk = typedHandle->Get();
			return chunk.get();
		}

		std::queue<SoundRequest> m_SoundQueue;

		void ProcessQueue()
		{
			while (!m_SoundQueue.empty())
			{
				auto& request = m_SoundQueue.front();

				int loops = request.looping ? -1 : 0;
				
				int channel = Mix_PlayChannel(-1, GetChunk(request.chunk), loops);

				if (channel == -1) // No available channels
					break;

				Mix_Volume(channel, static_cast<int>(request.volume * MIX_MAX_VOLUME));
				m_SoundQueue.pop();
			}
		}
		void ClearQueue()
		{
			std::queue<SoundRequest> empty;
			std::swap(m_SoundQueue, empty);
		}

		void RemoveFromQueue(Mix_Chunk* target)
		{
			std::queue<SoundRequest> temp;
			while (!m_SoundQueue.empty())
			{
				auto request = m_SoundQueue.front();
				m_SoundQueue.pop();
				if (GetChunk(request.chunk) != target)
				{
					temp.push(request);
				}
			}
			m_SoundQueue = std::move(temp);
		}

		bool m_Muted{ false };
		float m_volume{0};
	};

	SDLAudio::SDLAudio()
	{

		if (!m_pImpl.get())
			m_pImpl = std::make_unique<SDL_AudioImpl>();

	}

	SDLAudio::~SDLAudio()
	{

	}


	void SDLAudio::PlayEffect(sound_id soundId, const float volume, bool looping)
	{
		m_pImpl->PlaySound(soundId, volume, looping);
	}

	//void SDLAudio::PlayMusic(sound_id soundId, const float volume, bool looping)
	//{
	//	m_pImpl->PlayMusic(soundId, volume, looping);
	//}

	//void SDLAudio::SetMusicVolume(const float volume)
	//{
	//	m_pImpl->SetMusicVolume(volume);
	//}
	//void SDLAudio::SetSoundVolume(const float volume)
	//{
	//	m_pImpl->SetSoundVolume(volume);
	//}

	void SDLAudio::SetMasterVolume(const float volume)
	{
		m_pImpl->SetMasterVolume(volume);
	}

	std::shared_ptr<ISoundHandle> SDLAudio::LoadSound(const std::string& file)
	{
		return m_pImpl->LoadSound(file);
	}

	void SDLAudio::StopAllSounds()
	{
		m_pImpl->StopAllSounds();
	}

	void SDLAudio::StopSound(sound_id soundId)
	{
		m_pImpl->StopSound(soundId);
	}

	void SDLAudio::Mute()
	{
		m_pImpl->Mute();
	}


	LoggerAudio::LoggerAudio(std::unique_ptr<IAudio>&& pAudio)
		: m_pAudio(std::move(pAudio))
	{
		std::cout << "Logger audio created." << std::endl;
	}

	void LoggerAudio::PlayEffect(sound_id soundId, const float volume, bool looping)
	{
		m_pAudio->PlayEffect(soundId, volume, looping);
		std::cout << "Sound played: " << soundId << std::endl;
	}

	std::shared_ptr<ISoundHandle> LoggerAudio::LoadSound(const std::string& file) 
	{
		auto handle = m_pAudio->LoadSound(file);
		std::cout << "Sound loaded: " << file << std::endl;
		return handle;
	}

	/*void LoggerAudio::PlayMusic(sound_id soundId, const float volume, bool looping)
	{
		m_pAudio->PlayMusic(soundId, volume, looping);
		std::cout << "Music is playing: " << soundId << std::endl;
	}*/

	void LoggerAudio::SetMasterVolume(const float volume)
	{
		m_pAudio->SetMasterVolume(volume);
		std::cout << "Master volume set to: " << volume << std::endl;
	}

	void LoggerAudio::StopAllSounds()
	{
		m_pAudio->StopAllSounds();
		std::cout << "All sounds stopped." << std::endl;
	}

	/*void LoggerAudio::SetMusicVolume(const float volume)
	{
		m_pAudio->SetMusicVolume(volume);
		std::cout << "Music volume set to: " << volume << std::endl;
	}

	void LoggerAudio::SetSoundVolume(const float volume) 
	{
		m_pAudio->SetSoundVolume(volume);
		std::cout << "Sound volume set to: " << volume << std::endl;
	}*/

	void LoggerAudio::Mute()
	{
		m_pAudio->Mute();
		std::cout << "Sound mute state changed." << std::endl;
	}

	void LoggerAudio::StopSound(sound_id soundId)
	{
		m_pAudio->StopSound(soundId);
		std::cout << "Sound ID: " << soundId << " stopped." << std::endl;
	}
}