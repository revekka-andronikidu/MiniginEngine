#pragma once
#include "TextComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include <stdexcept>
#include <SDL_ttf.h>


dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color) : GraphicsComponent(pOwner)
, m_needsUpdate(true)
, m_text(text)
, m_font(std::move(font))
, m_color{ color }
{
	m_pTexture = nullptr;
	
}

void dae::TextComponent::Update()

{
	if (m_needsUpdate)
	{
		const SDL_Color color = m_color; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<dae::Texture2D>(texture);
		m_needsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color color)
{
	m_color = color;
	m_needsUpdate = true;
}