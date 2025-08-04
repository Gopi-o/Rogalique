#include "pch.h"
#include "Button.h"
#include <Systems/Logger.h>

namespace Engine
{
	Button::Button(GameObject* gameObject)
		: Widget(gameObject)
	{
		// Инициализация текста
		auto defaultFont = ResourceSystem::Instance()->GetFontShared("default_font");
		if (defaultFont)
		{
			buttonText.setFont(*defaultFont);
		}
		buttonText.setCharacterSize(16);
		buttonText.setFillColor(sf::Color::White);

		// Настройка обработчиков событий
		OnPressed = [this]() {
			if (isEnabled)
			{
				isPressedInternally = true;
				UpdateButtonAppearance();
			}
		};

		OnReleased = [this]() {
			if (isEnabled && isPressedInternally)
			{
				isPressedInternally = false;
				UpdateButtonAppearance();
				if (OnClick)
				{
					OnClick();
				}
			}
		};

		OnHovered = [this]() {
			if (isEnabled)
			{
				UpdateButtonAppearance();
			}
		};

		OnUnhovered = [this]() {
			if (isEnabled)
			{
				isPressedInternally = false;
				UpdateButtonAppearance();
			}
		};
	}

	Button::~Button()
	{
		OnClick = nullptr;
		OnHovered = nullptr;
		OnUnhovered = nullptr;
	}

	void Button::SetNormalTexture(const std::string& textureName)
	{
		normalTexture = ResourceSystem::Instance()->GetTextureShared(textureName);
		UpdateButtonAppearance();
	}

	void Button::SetHoveredTexture(const std::string& textureName)
	{
		hoveredTexture = ResourceSystem::Instance()->GetTextureShared(textureName);
		UpdateButtonAppearance();
	}

	void Button::SetPressedTexture(const std::string& textureName)
	{
		pressedTexture = ResourceSystem::Instance()->GetTextureShared(textureName);
		UpdateButtonAppearance();
	}

	void Button::SetDisabledTexture(const std::string& textureName)
	{
		disabledTexture = ResourceSystem::Instance()->GetTextureShared(textureName);
		UpdateButtonAppearance();
	}

	void Button::SetSize(const Vector2Df& newSize)
	{
		// Проверка на валидность размеров
		if (newSize.x <= 0 || newSize.y <= 0)
		{
			return;
		}

		Widget::SetSize(newSize);

		// Обновляем спрайт только если он используется
		if (useSprite)
		{
			SetSpriteSize(sprite, newSize.x, newSize.y);
		}

		UpdateTextPosition();
	}

	void Button::SetText(const std::string& text)
	{
		textString = text;
		buttonText.setString(textString);
		UpdateTextPosition();
	}

	void Button::SetTextFont(const std::string& fontName)
	{
		auto font = ResourceSystem::Instance()->GetFontShared(fontName);
		if (font)
		{
			buttonText.setFont(*font);
			UpdateTextPosition();
		}
	}

	void Button::SetTextSize(unsigned int size)
	{
		buttonText.setCharacterSize(size);
		UpdateTextPosition();
	}

	void Button::SetTextColor(const sf::Color& color)
	{
		buttonText.setFillColor(color);
	}

	void Button::SetTextOutlineColor(const sf::Color& color)
	{
		buttonText.setOutlineColor(color);
	}

	void Button::SetTextOutlineThicness(float thickness)
	{
		buttonText.setOutlineThickness(thickness);
	}

	void Button::SetTextHorizontalAlignment(EHorizontalAlignment alignment)
	{
		textAlignment = alignment;
		UpdateTextPosition();
	}

	void Button::SetTextPadding(float padding)
	{
		textPadding = padding;
		UpdateTextPosition();
	}

	void Button::SetEnabled(bool enabled)
	{
		if (isEnabled != enabled)
		{
			isEnabled = enabled;
			UpdateButtonAppearance();
		}
	}

	void Button::HandleEvent(const sf::Event& event)
	{
		if (!isEnabled || !IsVisible())
			return;

		Widget::HandleEvent(event);
	}

	void Button::Update(float deltaTime)
	{
		Widget::Update(deltaTime);
	}

	void Button::Render()
	{
		if (!IsVisible())
			return;

		// 1. Отладочный прямоугольник (границы виджета)
		sf::RectangleShape widgetRect(sf::Vector2f(GetSize().x, GetSize().y));
		widgetRect.setPosition(GetAbsolutePosition().x, GetAbsolutePosition().y);
		widgetRect.setFillColor(sf::Color::Transparent);
		widgetRect.setOutlineColor(sf::Color::Green);
		widgetRect.setOutlineThickness(1.f);
		RenderSystem::Instance()->Render(widgetRect);

		// 2. Рендеринг спрайта
		if (useSprite)
		{
			// Отладочные границы спрайта
			sf::FloatRect spriteBounds = sprite.getGlobalBounds();
			sf::RectangleShape spriteRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
			spriteRect.setPosition(spriteBounds.left, spriteBounds.top);
			spriteRect.setFillColor(sf::Color::Transparent);
			spriteRect.setOutlineColor(sf::Color::Red);
			spriteRect.setOutlineThickness(1.f);
			RenderSystem::Instance()->Render(spriteRect);

			RenderSystem::Instance()->Render(sprite);
		}

		// 3. Рендеринг текста
		RenderSystem::Instance()->Render(buttonText);
	}

	
	void Button::UpdateButtonAppearance()
	{
		// Определяем, какую текстуру использовать
		const sf::Texture* currentTexture = nullptr;

		if (!isEnabled && disabledTexture)
		{
			currentTexture = disabledTexture;
		}
		else if (isPressedInternally && pressedTexture)
		{
			currentTexture = pressedTexture;
		}
		else if (GetIsHovered() && hoveredTexture)
		{
			currentTexture = hoveredTexture;
		}
		else if (normalTexture)
		{
			currentTexture = normalTexture;
		}

		// Если текстура найдена - настраиваем спрайт
		if (currentTexture)
		{
			useSprite = true;
			sprite.setTexture(*currentTexture, true);

			// Устанавливаем origin в центр спрайта
			sf::FloatRect bounds = sprite.getLocalBounds();
			sprite.setOrigin(bounds.width / 2, bounds.height / 2);

			// Устанавливаем scale для нужного размера
			SetSpriteSize(sprite, GetSize().x, GetSize().y);

			// Позиционируем по центру виджета
			Vector2Df absPos = GetAbsolutePosition();
			sprite.setPosition(absPos.x + GetSize().x / 2, absPos.y + GetSize().y / 2);
		}
		else
		{
			useSprite = false;
		}

		UpdateTextPosition();
	}

	
    void Button::UpdateTextPosition()
	{
		if (textString.empty())
			return;

		const auto textBounds = buttonText.getLocalBounds();
		const float buttonWidth = GetSize().x;
		const float buttonHeight = GetSize().y;
		const float outlineThickness = buttonText.getOutlineThickness();

		// Сбрасываем origin перед вычислениями
		buttonText.setOrigin(0, 0);

		// Вертикальное центрирование
		float textY = (buttonHeight - textBounds.height) / 2 - textBounds.top;

		// Горизонтальное выравнивание
		float textX = 0.f;
		switch (textAlignment)
		{
			case EHorizontalAlignment::Left:
				textX = textPadding;
				break;
			case EHorizontalAlignment::Right:
				textX = buttonWidth - textBounds.width - textPadding;
				break;
			case EHorizontalAlignment::Center:
			default:
				textX = (buttonWidth - textBounds.width) / 2;
				break;
		}

		// Учитываем outline
		textX += outlineThickness;
		textY += outlineThickness;

		Vector2Df widgetPos = GetAbsolutePosition();
		buttonText.setPosition(widgetPos.x + textX, widgetPos.y + textY);
	}

	void Button::UpdateTransform()
	{
		if (isUpdatingTransform)
			return;
		Widget::UpdateTransform();
		if (useSprite)
		{
			Vector2Df absPos = GetAbsolutePosition();
			sprite.setPosition(absPos.x + GetSize().x / 2, absPos.y + GetSize().y / 2);
		}
		UpdateButtonAppearance();
		UpdateTextPosition();
		LOG_DEBUG("Button transform - Pos: " + std::to_string(GetAbsolutePosition().x) + "," + std::to_string(GetAbsolutePosition().y) + " Size: " + std::to_string(GetSize().x) + "x" + std::to_string(GetSize().y));
	}
}