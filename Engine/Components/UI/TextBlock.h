#pragma once
#include <Components/UI/Widget.h>
#include <Math/Vector.h>

namespace Engine
{
	enum class EHorizontalAlignment
	{
		Left,
		Center,
		Right
	};

	enum class EVerticalAlignment
	{
		Top,
		Center,
		Right,
		Bottom
	};

	class TextBlock : public Widget
	{
	public:
		TextBlock(GameObject* gameObject);

		void Render() override;
		Vector2Df CalculateDesiredSize() const override;

		void SetText(const std::string& text);
		const std::string& GetText() const { return textString; }

		void SetFont(const std::string& fontName);
		void SetFontSize(unsigned int size);
		unsigned int GetFontSize() const { return text.getCharacterSize(); }

		void SetTextColor(const sf::Color& color);
		void SetOutlineColor(const sf::Color& color);
		void SetOutlineThickness(float thickness);

		void SetHorizontalAlignment(EHorizontalAlignment alignment);
		void SetVerticalAlignment(EVerticalAlignment alignment);

	protected:
		void UpdateTransform() override;
		void UpdateTextPosition();
		sf::FloatRect GetAdjustedTextBounds() const;

	private:
		sf::Text text;
		std::string textString;

		EHorizontalAlignment horizontalAlign = EHorizontalAlignment::Left;
		EVerticalAlignment verticalAlign = EVerticalAlignment::Top;
	};
} // namespace Engine
