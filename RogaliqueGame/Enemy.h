#pragma once
#include "GameObject.h"
#include <TransformComponent.h>
#include <RigidbodyComponent.h>
#include "Vector.h"
#include <SpriteRendererComponent.h>

namespace RogaliqueGame
{
    class Enemy
    {
    public:
        Enemy();
        ~Enemy();

        void Update(float deltaTime);
        Engine::GameObject* GetGameObject() const { return gameObject; }

    private:
        Engine::GameObject* gameObject;
        float moveSpeed;
        float detectionRange;

        void FindAndChasePlayer();
    };
}