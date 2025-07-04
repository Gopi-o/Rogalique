#include "pch.h"
#include "TextBlock.h"
#include <Systems/Resource/ResourceSystem.h>
#include <Systems/Render/RenderSystem.h>

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
		UpdateTextPosition();
		Widget::UpdateTransform();
	}

	void TextBlock::UpdateTextPosition()
	{
		const auto textBounds = text.getLocalBounds();

		float xPos = GetPosition().x;
		float yPos = GetPosition().y;

		// Горизонтальное выравнивание
		switch (horizontalAlign)
		{
			case EHorizontalAlignment::Left:
				text.setOrigin(0, text.getOrigin().y);
				break;
			case EHorizontalAlignment::Center:
				text.setOrigin(textBounds.width / 2.0f, text.getOrigin().y);
				xPos += GetSize().x / 2.0f;
				break;
			case EHorizontalAlignment::Right:
				text.setOrigin(textBounds.width, text.getOrigin().y);
				xPos += GetSize().x;
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
				yPos += GetSize().y / 2.0f;
				break;
			case EVerticalAlignment::Bottom:
				text.setOrigin(text.getOrigin().x, textBounds.height);
				yPos += GetSize().y;
				break;
		}

		text.setPosition(xPos, yPos);
	}

} // namespace Engine