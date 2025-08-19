#include "pch.h"
#include "SpriteRendererComponent.h"
#include <Systems/Render/RenderSystem.h>

namespace Engine
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		sprite = new sf::Sprite();
		scale = { 1, -1 };
		sprite->setScale({ 1, -1 });
		transform = gameObject->GetComponent<TransformComponent>();
	}

	SpriteRendererComponent::~SpriteRendererComponent()
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}

	void SpriteRendererComponent::Update(float deltaTime)
	{
	}

	void SpriteRendererComponent::Render()
	{
		if (sprite != nullptr)
		{
			sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(transform->GetWorldPosition()));
			sprite->setRotation(transform->GetWorldRotate());

			auto transformScale = Convert<sf::Vector2f, Vector2Df>(transform->GetWorldScale());
			sprite->setScale({ scale.x * transformScale.x, scale.y * transformScale.y });
			RenderSystem::Instance()->Render(*sprite);
		}
	}

	const sf::Sprite* SpriteRendererComponent::GetSprite() const
	{
		return sprite;
	}

	void SpriteRendererComponent::SetTexture(const sf::Texture& newTexture)
	{
		sprite->setTexture(newTexture);
		auto textureSize = sprite->getTexture()->getSize();
		sprite->setOrigin({ 0.5f * textureSize.x, 0.5f * textureSize.y });
	}

	void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
	{
		auto tex = sprite->getTexture();
		if (!tex)
			return;
		sf::IntRect rect = sprite->getTextureRect();
		sf::Vector2u base = (rect.width > 0 && rect.height > 0)
			? sf::Vector2u(rect.width, rect.height)
			: tex->getSize();
		scale = { (float)newWidth / (float)base.x, -(float)newHeight / (float)base.y };
	};

	void SpriteRendererComponent::FlipX(bool flip)
	{
		if (flip != isFlipX)
		{
			/*auto scale = sprite->getScale();
			sprite->setScale({ -scale.x, scale.y });*/
			scale = { -scale.x, scale.y };
			isFlipX = flip;
		}
	}

	void SpriteRendererComponent::FlipY(bool flip)
	{
		if (flip != isFlipY)
		{
			scale = { scale.x, -scale.y };
			isFlipY = flip;
		}
	}

} // namespace Engine