#pragma once
#include "TransformComponent.h"
#include "Vector.h"
#include <SFML/Graphics.hpp>
#include <memory>


namespace Engine
{
	class SpriteRendererComponent : public Component
	{
	public:
		SpriteRendererComponent(GameObject* gameObject);
		~SpriteRendererComponent();

		void Update(float deltaTime) override;
		void Render() override;

		const sf::Sprite* GetSprite() const;
		void SetTexture(const sf::Texture& newTexture);
		void SetPixelSize(int newWidth, int newHeight);

		void FlipX(bool flip);
		void FlipY(bool flip);

	private:
		Vector2Df scale;
		sf::Sprite* sprite;
		TransformComponent* transform;

		bool isFlipX = false;
		bool isFlipY = false;

	};
}

