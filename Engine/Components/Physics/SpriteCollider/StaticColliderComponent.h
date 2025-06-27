#pragma once
#include <Components/Physics/SpriteCollider/SpriteColliderComponent.h>
#include <Components/Physics/Rigidbody/RigidbodyComponent.h>

namespace Engine
{
	class StaticColliderComponent : public SpriteColliderComponent
	{
	public:
		StaticColliderComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;
	};
} // namespace Engine
