#include "pch.h"
#include "Image.h"
#include <Systems/Resource/ResourceSystem.h>

namespace Engine
{
	Image::Image(GameObject* gameObject)
		: Widget(gameObject)
	{
		useSprite = true;
		SetSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
	}

	void Image::SetTexture(const std::string& textureName)
	{
		if (textureName == currentTextureName)
			return;

		const sf::Texture* texture = ResourceSystem::Instance()->GetTextureShared(textureName);
		if (texture)
		{
			SetTexture(*texture);
			currentTextureName = textureName;
		}
	}

	void Image::SetTexture(const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		SetSpriteSize(sprite, GetSize().x, GetSize().y);
		UpdateTransform();
	}

	void Image::SetSize(const Vector2Df& newSize)
	{
		Widget::SetSize(newSize);
		SetSpriteSize(sprite, newSize.x, newSize.y);
	}

	void Image::SetOrigin(float originX, float originY)
	{
		SetSpriteRelativeOrigin(sprite, originX, originY);
		UpdateTransform();
	}

	void Image::SetColor(const sf::Color& color)
	{
		sprite.setColor(color);
	}

	const sf::Color& Image::GetColor() const
	{
		return sprite.getColor();
	}

	void Image::SetRotation(float angle)
	{
		sprite.setRotation(angle);
	}

	float Image::GetRotation() const
	{
		return sprite.getRotation();
	}

	Vector2Df Image::CalculateDesiredSize() const
	{
		if (sprite.getTexture())
		{
			return { sprite.getGlobalBounds().width, sprite.getGlobalBounds().height };
		}
		return GetSize();
	}

	void Image::Render()
	{
		if (!IsVisible() || !sprite.getTexture())
			return;

		RenderSystem::Instance()->Render(sprite);
		Widget::Render();
	}

	void Image::UpdateTransform()
	{
		sprite.setPosition(GetPosition().x, GetPosition().y);
		Widget::UpdateTransform();
	}
} // namespace Engine