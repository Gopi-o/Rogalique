#include "pch.h"
#include "StaticColliderComponent.h"

namespace Engine
{
	StaticColliderComponent::StaticColliderComponent(GameObject* gameObject)
		: SpriteColliderComponent(gameObject)
	{
		auto rigidBody = gameObject->GetComponent<RigidbodyComponent>();
		if (!rigidBody)
		{
			rigidBody = gameObject->AddComponent<RigidbodyComponent>();
		}
		rigidBody->SetKinematic(true);
		rigidBody->SetLinearDamping(8.1f);
	}

	void StaticColliderComponent::Update(float deltaTime)
	{
		SpriteColliderComponent::Update(deltaTime);
	}

	void StaticColliderComponent::Render()
	{
		SpriteColliderComponent::Render();
	}

} // namespace Engine