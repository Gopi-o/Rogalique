#pragma once
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"

namespace Engine
{
	class FloorComponent : public SpriteColliderComponent
	{
	public:
		FloorComponent(GameObject* gameObject);
		~FloorComponent();

		void Update(float deltaTime) override;
		void Render() override;
	};
}

