#pragma once
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"

namespace Engine
{
	class WallComponent : public SpriteColliderComponent
	{
	public:
		WallComponent(GameObject* gameObject);
		~WallComponent();

		void Update(float deltaTime) override;
		void Render() override;
	};
}

