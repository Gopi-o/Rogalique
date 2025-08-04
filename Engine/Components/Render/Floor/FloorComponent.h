#pragma once
#include "Components/Physics/SpriteCollider/StaticColliderComponent.h"
#include "Components/Physics/Rigidbody/RigidbodyComponent.h"

namespace Engine
{
	class FloorComponent : public Component
	{
	public:
		FloorComponent(GameObject* gameObject);
		~FloorComponent();

		void Update(float deltaTime) override;
		void Render() override;

		virtual bool IsPassible() const { return true; }
	};
} // namespace Engine
