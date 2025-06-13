#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "Font.h"

namespace dae
{
	class TextComponent final: public GraphicsComponent
	{
	public:
		class Texture2D;

		virtual void Update() override;		

		void SetText(const std::string& text);
		void SetColor(const SDL_Color color);

		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color = {255,255,255,255});
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		SDL_Color m_color;
	};
}

