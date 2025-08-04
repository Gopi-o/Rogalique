#include "pch.h"
#include "TextBlock.h"
#include <Systems/Resource/ResourceSystem.h>
#include <Systems/Render/RenderSystem.h>
#include "Button.h"

namespace Engine
{
	TextBlock::TextBlock(GameObject* gameObject)
		: Widget(gameObject)
	{
	}

	void TextBlock::Render()
	{
		if (!IsVisible())
			return;

		// Only render if not in a canvas

		RenderSystem::Instance()->Render(text);

		Widget::Render();
	}

	Vector2Df TextBlock::CalculateDesiredSize() const
	{
		const auto& bounds = text.getGlobalBounds();
		return { bounds.width, bounds.height };
	}

	void TextBlock::SetText(const std::string& text)
	{
		textString = text;
		this->text.setString(textString);
		UpdateTextPosition();
	}

	void TextBlock::SetFont(const std::string& fontName)
	{
		auto font = ResourceSystem::Instance()->GetFontShared(fontName);
		if (font)
		{
			text.setFont(*font);
			UpdateTextPosition();
		}
	}

	void TextBlock::SetFontSize(unsigned int size)
	{
		text.setCharacterSize(size);
		UpdateTextPosition();
	}

	void TextBlock::SetTextColor(const sf::Color& color)
	{
		text.setFillColor(color);
	}

	void TextBlock::SetOutlineColor(const sf::Color& color)
	{
		text.setOutlineColor(color);
	}

	void TextBlock::SetOutlineThickness(float thickness)
	{
		text.setOutlineThickness(thickness);
		UpdateTextPosition();
	}

	void TextBlock::SetHorizontalAlignment(EHorizontalAlignment alignment)
	{
		horizontalAlign = alignment;
		UpdateTextPosition();
	}

	void TextBlock::SetVerticalAlignment(EVerticalAlignment alignment)
	{
		verticalAlign = alignment;
		UpdateTextPosition();
	}

	void TextBlock::UpdateTransform()
	{
		Widget::UpdateTransform();
		UpdateTextPosition();
	}

	void TextBlock::UpdateTextPosition()
	{
		const auto textBounds = GetAdjustedTextBounds();
		const Vector2Df absolutePos = GetAbsolutePosition();
		Vector2Df textPos = absolutePos;

		// Горизонтальное выравнивание
		switch (horizontalAlign)
		{
			case EHorizontalAlignment::Left:
				text.setOrigin(0, text.getOrigin().y);
				break;

			case EHorizontalAlignment::Center:
				text.setOrigin(textBounds.width / 2.0f, text.getOrigin().y);
				textPos.x += GetSize().x / 2.0f;
				break;

			case EHorizontalAlignment::Right:
				text.setOrigin(textBounds.width, text.getOrigin().y);
				textPos.x += GetSize().x;
				break;
		}

		// Вертикальное выравнивание
		switch (verticalAlign)
		{
			case EVerticalAlignment::Top:
				text.setOrigin(text.getOrigin().x, 0);
				break;

			case EVerticalAlignment::Center:
				text.setOrigin(text.getOrigin().x, textBounds.height / 2.0f);
				textPos.y += GetSize().y / 2.0f;
				break;

			case EVerticalAlignment::Bottom:
				text.setOrigin(text.getOrigin().x, textBounds.height);
				textPos.y += GetSize().y;
				break;
		}

		text.setPosition(textPos.x, textPos.y);
	}

	sf::FloatRect TextBlock::GetAdjustedTextBounds() const
	{
		auto bounds = text.getLocalBounds();
		const float outline = text.getOutlineThickness();
		return {
			bounds.left - outline,
			bounds.top - outline,
			bounds.width + outline * 2,
			bounds.height + outline * 2
		};
	}

} // namespace Engine