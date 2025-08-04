#pragma once
#include "Components/Physics/SpriteCollider/StaticColliderComponent.h"
#include "Components/Physics/Rigidbody/RigidbodyComponent.h"

namespace Engine
{
	class WallComponent : public StaticColliderComponent
	{
	public:
		WallComponent(GameObject* gameObject);
		~WallComponent();

		void Update(float deltaTime) override;
		void Render() override;
	};
} // namespace Engine
