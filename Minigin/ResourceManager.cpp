#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "ServiceLocator.h"
#include "SoundHandle.h"
#include <iostream>

namespace fs = std::filesystem;

void dae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if(m_loadedTextures.find(filename) == m_loadedTextures.end())
		m_loadedTextures.insert(std::pair(filename,std::make_shared<Texture2D>(fullPath.string())));
	return m_loadedTextures.at(filename);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_loadedFonts.find(key) == m_loadedFonts.end())
		m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
	return m_loadedFonts.at(key);
}

std::shared_ptr<dae::Font> dae::ResourceManager::GetFont(const std::string& file, uint8_t size)
{
	const auto filename = fs::path(file).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);

	auto it = m_loadedFonts.find(key);
	if (it != m_loadedFonts.end())
	{
		return it->second;
	}

	throw std::runtime_error("Font not loaded: " + filename + " (size " + std::to_string(size) + ")");
}

std::shared_ptr<dae::Font> dae::ResourceManager::GetFont(const std::string& file)
{
	const auto filename = fs::path(file).filename().string();

	for (const auto& [key, font] : m_loadedFonts)
	{
		if (key.first == filename)
			return font;
	}

	throw std::runtime_error("Font not loaded (any size): " + filename);
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::GetTexture(const std::string& file)
{
	auto it = m_loadedTextures.find(file);

	if (it != m_loadedTextures.end())
	{
		return it->second;
	}
	throw std::runtime_error("Texture not loaded: " + file);
}

void dae::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedTextures.erase(it);
		else
			++it;
	}

	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedFonts.erase(it);
		else
			++it;
	}

	for (auto it = m_Sounds.begin(); it != m_Sounds.end();)
	{
		if (it->second.use_count() == 1)
			it = m_Sounds.erase(it);
		else
			++it;
	}
}

std::shared_ptr<dae::ISoundHandle> dae::ResourceManager::LoadSound(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	//const auto filename = fs::path(fullPath).filename().string();

	if (auto it = m_Sounds.find(file); it != m_Sounds.end()) 
	{
		return it->second; // Return existing sound
	}

	auto sound = ServiceLocator::GetAudioService().LoadSound(fullPath.string());


	m_Sounds[file] = sound;


	sound_id id = m_NextSoundId++;

	// Store in maps
	m_FileToId[id] = file;

	
	return m_Sounds.at(file);
}


