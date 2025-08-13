#pragma once
#include "TextComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include <stdexcept>
#include <SDL_ttf.h>


dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color, TextAlign aligment) : GraphicsComponent(pOwner)
, m_needsUpdate(true)
, m_text(text)
, m_font(std::move(font))
, m_color{ color }
, m_alignment{aligment}
{
	m_pTexture = nullptr;
	
}

glm::ivec2 dae::TextComponent::GetTextureSize() const
{
	return m_pTexture->GetSize();
}

void dae::TextComponent::Update()

{
	if (m_needsUpdate)
	{
		const SDL_Color color = m_color; 
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

void dae::TextComponent::Render() const
{
	if (m_pTexture == nullptr)
		return;

	glm::vec3 pos = GetOwner()->GetTransform().GetWorldPosition();
	pos.y += m_VerticalOffset;
	glm::vec3 scale = GetOwner()->GetTransform().GetScale();

	int textureWidth = m_pTexture->GetSize().x;

	switch (m_alignment)
	{
	case TextAlign::Left:
		// no offset
		break;
	case TextAlign::Center:
		pos.x -= (textureWidth * 0.5f) * scale.x;
		break;
	case TextAlign::Right:
		pos.x -= (textureWidth)*scale.x;
		break;
	}

	
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, scale);
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

void dae::TextComponent::SetAlign(const TextAlign aligment)
{
	m_alignment = aligment;
	m_needsUpdate = true;
}

void dae::TextComponent::SetFont(const std::shared_ptr<Font> font)
{
	m_font = font;
	m_needsUpdate = true;

}

void dae::TextComponent::SetVerticalOffset(float offset) 
{ 
	m_VerticalOffset = offset; 
}