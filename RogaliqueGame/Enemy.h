#pragma once
#include "GameObject.h"
#include <TransformComponent.h>
#include <RigidbodyComponent.h>
#include "Vector.h"
#include <SpriteRendererComponent.h>
#include <DamageableComponent.h>

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
        float attackRange;
        float attackCooldown;
        float currentCooldown;
        float attackDamage;

        void FindAndChasePlayer();
        void AttackPlayer(Engine::GameObject* player);
    };
}