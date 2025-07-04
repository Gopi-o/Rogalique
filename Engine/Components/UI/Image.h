#pragma once
#include <Components/UI/Widget.h>
#include <ResourceSystem/Sprite.h>
#include <SFML/Graphics/Sprite.hpp>

namespace Engine
{
	class Image : public Widget
	{
	public:
		Image(GameObject* gameObject);

		void SetTexture(const std::string& textureName);
		void SetTexture(const sf::Texture& texture);

		void SetSize(const Vector2Df& newSize) override;
		void SetOrigin(float originX, float originY);

		void SetColor(const sf::Color& color);
		const sf::Color& GetColor() const;
		void SetRotation(float angle);
		float GetRotation() const;

		Vector2Df CalculateDesiredSize() const override;
		void Render() override;

	private:
		void UpdateTransform() override;

		sf::Sprite sprite;
		std::string currentTextureName;
	};
} // namespace Engine
