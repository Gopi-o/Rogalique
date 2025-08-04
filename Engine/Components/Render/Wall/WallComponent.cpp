#include "pch.h"
#include "WallComponent.h"

namespace Engine
{
	WallComponent::WallComponent(GameObject* gameObject)
		: StaticColliderComponent(gameObject)
	{
	}

	WallComponent::~WallComponent()
	{
	}

	void WallComponent::Update(float deltaTime)
	{
		StaticColliderComponent::Update(deltaTime);
	}

	void WallComponent::Render()
	{
		StaticColliderComponent::Render();
	}

} // namespace Engine
