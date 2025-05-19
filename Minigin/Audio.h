#pragma once
#include <memory>
#include <mutex>
#include <queue>

namespace dae
{
	using sound_id = unsigned short;
	class ISoundHandle;

	class IAudio 
	{
	public:
		virtual ~IAudio() = default;


		virtual std::shared_ptr<ISoundHandle> LoadSound(const std::string& file) = 0;

		virtual void PlayEffect(sound_id soundId, const float volume, bool looping = 0) = 0;
		//virtual void PlayMusic(sound_id soundId, const float volume, bool looping = 1) = 0;
		virtual void SetMasterVolume(const float volume) = 0;
		virtual void StopAllSounds() = 0;
		virtual void StopSound(sound_id soundId) = 0;

		//virtual void SetMusicVolume(const float volume) = 0;
		//virtual void SetSoundVolume(const float volume) = 0;

		//private:
		//master vol
		//sound vol
		
	};

	class SDLAudio final : public IAudio
	{

		class SDL_AudioImpl;
		std::unique_ptr<SDL_AudioImpl> m_pImpl{};
	public:
		SDLAudio();
		virtual ~SDLAudio() override;
		SDLAudio(const SDLAudio& other) = delete;
		SDLAudio(SDLAudio&& other) = delete;
		SDLAudio& operator=(const SDLAudio& other) = delete;
		SDLAudio& operator=(SDLAudio&& other) = delete;

		std::shared_ptr<ISoundHandle> LoadSound(const std::string& file) override;
		void PlayEffect(sound_id soundId, const float volume, bool looping = 0) override;
		//void PlayMusic(sound_id soundId, const float volume, bool looping = 1) override;
		void SetMasterVolume(const float volume) override;
		void StopAllSounds() override;
		void StopSound(sound_id soundId) override;

		//void SetMusicVolume(const float volume);
		//void SetSoundVolume(const float volume);

	private:
		static std::mutex mutex;

	};

	class NullAudio final : public IAudio
	{
	public:
		void PlayEffect([[maybe_unused]] sound_id soundId, [[maybe_unused]] const float volume, [[maybe_unused]] bool looping = 0) override { };
		//void PlayMusic([[maybe_unused]] sound_id soundId, [[maybe_unused]] const float volume, [[maybe_unused]] bool looping = 1) override { };
		void SetMasterVolume([[maybe_unused]] const float volume) override {  };
		void StopAllSounds() override {};
		void StopSound([[maybe_unused]] sound_id soundId) override {  };
		std::shared_ptr<ISoundHandle> LoadSound([[maybe_unused]] const std::string& file) override { return nullptr; };

		//void SetMusicVolume([[maybe_unused]] const float volume)override {};
		//void SetSoundVolume([[maybe_unused]] const float volume) override {};
	};

	class LoggerAudio final : public IAudio
	{
	public:
		LoggerAudio(std::unique_ptr<IAudio>&& pAudio);

		~LoggerAudio() = default;
		std::shared_ptr<ISoundHandle> LoadSound(const std::string& file) override;
		void PlayEffect(sound_id soundId, const float volume, bool looping = 0) override;
		//void PlayMusic(sound_id soundId, const float volume, bool looping = 1) override;
		void SetMasterVolume(const float volume) override;
		void StopAllSounds() override;
		void StopSound(sound_id soundId) override;

		//void SetMusicVolume(const float volume)override;
		//void SetSoundVolume(const float volume) override;

	private:
		std::unique_ptr<IAudio> m_pAudio;
	};

}