#include "pch.h"
#include "FloorComponent.h"


namespace Engine
{
    FloorComponent::FloorComponent(GameObject* gameObject) : SpriteColliderComponent(gameObject)
    {
        auto rigidbody = gameObject->GetComponent<RigidbodyComponent>();
        if (!rigidbody)
        {
            rigidbody = gameObject->AddComponent<RigidbodyComponent>();
        }
        rigidbody->SetKinematic(true);
    }

    FloorComponent::~FloorComponent()
    {
    }

    void FloorComponent::Update(float deltaTime)
    {
        SpriteColliderComponent::Update(deltaTime);
    }

    void FloorComponent::Render()
    {
        SpriteColliderComponent::Render();
    }
}