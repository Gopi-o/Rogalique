#pragma once

#include "Components/Physics/Collider/ColliderComponent.h"
#include "Components/Render/SpriteRenderer/SpriteRendererComponent.h"
#include <Systems/Render/RenderSystem.h>
#include <SFML/Graphics/Sprite.hpp>

namespace Engine
{
	class SpriteColliderComponent : public ColliderComponent
	{
	public:
		SpriteColliderComponent(GameObject* gameObject);
		~SpriteColliderComponent();

		void Update(float deltaTime) override;
		void Render() override;

	private:
		const sf::Sprite* sprite;
		sf::FloatRect previousBounds;
	};
} // namespace Engine
