#include "pch.h"
#include "WallComponent.h"

namespace Engine
{
	WallComponent::WallComponent(GameObject* gameObject) : SpriteColliderComponent(gameObject)
	{
		auto rigidbody = gameObject->GetComponent<RigidbodyComponent>();
		if (!rigidbody)
		{
			rigidbody = gameObject->AddComponent<RigidbodyComponent>();
		}
		rigidbody->SetKinematic(true);
	}

	WallComponent::~WallComponent()
	{
	}

	void WallComponent::Update(float deltaTime)
	{
		SpriteColliderComponent::Update(deltaTime);
	}

	void WallComponent::Render()
	{
		SpriteColliderComponent::Render();
	}

}
