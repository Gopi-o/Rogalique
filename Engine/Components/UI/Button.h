#pragma once
#include <Components/UI/Widget.h>
#include <Systems/Resource/ResourceSystem.h>
#include "TextBlock.h"

namespace Engine
{
	class Button : public Widget
	{
	public:
		Button(GameObject* gameObject);
		~Button();

		// Установка текстур для разных состояний кнопки
		void SetNormalTexture(const std::string& textureName);
		void SetHoveredTexture(const std::string& textureName);
		void SetPressedTexture(const std::string& textureName);
		void SetDisabledTexture(const std::string& textureName);
		void SetSize(const Vector2Df& newSize) override;

		// Установка текста кнопки
		void SetText(const std::string& text);
		void SetTextFont(const std::string& fontName);
		void SetTextSize(unsigned int size);
		void SetTextColor(const sf::Color& color);
		void SetTextOutlineColor(const sf::Color& color);
		void SetTextOutlineThicness(float thickness);
		void SetTextHorizontalAlignment(EHorizontalAlignment alignment);
		void SetTextPadding(float padding);


		// Состояние кнопки
		void SetEnabled(bool enabled);
		bool IsEnabled() const { return isEnabled; }


		 // Обработка событий
		void HandleEvent(const sf::Event& event) override;
		void Update(float deltaTime) override;
		void Render() override;

		// События кнопки
		std::function<void()> OnClick;

	protected:
		void UpdateButtonAppearance();
		void UpdateTextPosition();
		void UpdateTransform() override;

	private:
		// Текстуры для разных состояний
		const sf::Texture* normalTexture = nullptr;
		const sf::Texture* hoveredTexture = nullptr;
		const sf::Texture* pressedTexture = nullptr;
		const sf::Texture* disabledTexture = nullptr;

		// Текст кнопки
		sf::Text buttonText;
		std::string textString;
		EHorizontalAlignment textAlignment = EHorizontalAlignment::Left;
		float textPadding = 10.f;

		// Состояния
		bool isEnabled = true;
		bool isPressedInternally = false;
	};
}
