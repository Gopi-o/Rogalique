#include "pch.h"
#include "Enemy.h"
#include <GameWorld.h>
#include <ResourceSystem.h>
#include <LevelEditor.h>

namespace RogaliqueGame
{
    Enemy::Enemy()
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject();
        gameObject->SetTag("Enemy");

        auto transform = gameObject->AddComponent<Engine::TransformComponent>();
        auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();

        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("enemy"));
        renderer->SetPixelSize(40, 40);

        moveSpeed = 0.35f;
        detectionRange = 300.0f;
    }

    Enemy::~Enemy()
    {
    }

    void Enemy::Update(float deltaTime)
    {
        FindAndChasePlayer();
    }

    void Enemy::FindAndChasePlayer()
    {
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        if (!transform)
        {
            std::cout << "Enemy transform is null!" << std::endl;
            return;
        }

        auto objectsInRange = Engine::GameWorld::Instance()->FindObjectsInRadius(
            transform->GetWorldPosition(),
            detectionRange
        );



        Engine::GameObject* player = nullptr;
        /*if (player)
        {
            std::cout << "Found object with tag: " << player->GetTag() << std::endl;
        }
        else
        {
            std::cout << "No objects found in range" << std::endl;
        }*/

        for (auto obj : objectsInRange)
        {
            if (obj->GetTag() == "Player")
            {
                player = obj;
                break;
            }
        }

        if (player)
        {
            auto playerTransform = player->GetComponent<Engine::TransformComponent>();
            if (!playerTransform) return;

            Engine::Vector2Df myPos = transform->GetWorldPosition();
            Engine::Vector2Df playerPos = playerTransform->GetWorldPosition();
            Engine::Vector2Df direction = { playerPos.x - myPos.x, playerPos.y - myPos.y };

            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0)
            {
                direction.x /= length;
                direction.y /= length;
            }

            auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
            if (rigidbody)
            {
                Engine::Vector2Df velocity = { direction.x * moveSpeed, direction.y * moveSpeed };
                rigidbody->SetLinearVelocity(velocity);
            }
        }
        else
        {
            auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
            if (rigidbody)
            {
                rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
            }
        }
    }
}