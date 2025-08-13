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

		enum class TextAlign
		{
			Left,
			Center,
			Right
		};

		virtual void Update() override;		

		void SetText(const std::string& text);
		void SetColor(const SDL_Color color);
		void SetFont(const std::shared_ptr<Font> font);
		void SetAlign(const TextAlign aligment);
		void SetVerticalOffset(float offset);
		glm::ivec2 GetTextureSize() const override;
		void Render() const override;

		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color = {255,255,255,255}, TextAlign aligment = TextAlign::Left);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;



	private:
		bool m_needsUpdate;
		float m_VerticalOffset{ 0.f };
		std::string m_text;
		std::shared_ptr<Font> m_font;
		SDL_Color m_color;
		TextAlign m_alignment;
	};
}

