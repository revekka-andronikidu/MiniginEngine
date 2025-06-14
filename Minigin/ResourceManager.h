#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include "Singleton.h"


namespace dae
{
	class Texture2D;
	class Font;
	class ISoundHandle;
	using sound_id = unsigned short;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Texture2D> GetTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, uint8_t size);
		std::shared_ptr<dae::Font> GetFont(const std::string& file, uint8_t size);
		std::shared_ptr<dae::Font> GetFont(const std::string& file);
		//std::shared_ptr<ISoundHandle> LoadMusic(const std::string& file);

		const std::filesystem::path GetDataPath() { return m_dataPath; };

		std::shared_ptr<ISoundHandle> LoadSound(const std::string& file);
		std::shared_ptr<ISoundHandle> GetSound(const std::string& file) const
		{ 
			auto it = m_Sounds.find(file);

			if (it != m_Sounds.end())
			{
				return it->second;
			}
			throw std::runtime_error("Sound not loaded: " + file);
			//return nullptr; // Sound not found
		}

		std::shared_ptr<ISoundHandle> GetSound(sound_id id) const
		{
			auto it = m_FileToId.find(id);

			if (it != m_FileToId.end())
			{
				auto file = it->second;
				return GetSound(file);
			}
			
			throw std::runtime_error("Sound not found by ID: " + std::to_string(id));
		}

		



	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::filesystem::path m_dataPath;

		void UnloadUnusedResources();

		std::map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;

		std::map<const std::string, std::shared_ptr<ISoundHandle>> m_Sounds;
		std::unordered_map<sound_id, std::string> m_FileToId;

		sound_id m_NextSoundId{ 0 };


	};
}
