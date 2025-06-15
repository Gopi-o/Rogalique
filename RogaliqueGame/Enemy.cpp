#include "pch.h"
#include "Enemy.h"
#include <GameWorld.h>
#include <ResourceSystem.h>
#include <LevelEditor.h>
#include "Logger.h"
#include <EffectComponent.h>
#include "UnitStatsComponent.h"
#include <SoundManagerComponent.h>




namespace RogaliqueGame
{
    Enemy::Enemy()
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject();
        gameObject->SetTag("Enemy");
        LOG_INFO("Enemy created");

        auto transform = gameObject->AddComponent<Engine::TransformComponent>();
        auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
        auto damageable = gameObject->AddComponent<Engine::DamageableComponent>();
        auto EnemyStats = gameObject->AddComponent<UnitStatsComponent>();
        auto effectComponent = gameObject->AddComponent<Engine::EffectComponent>();

        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("enemy"));
        renderer->SetPixelSize(40, 40);

        auto soundManager = gameObject->AddComponent<Engine::SoundManagerComponent>();

        soundManager->AddSound("kill", "Resources\\Sounds\\Kill.wav", 50.0f, false);

        moveSpeed = 0.35f;
        detectionRange = 300.0f;
        attackRange = 50.0f;
        attackCooldown = 1.0f;
        currentCooldown = 0.0f;
        attackDamage = 10.0f;

        EnemyStats->SetHealth(50.f);
        LOG_DEBUG("Enemy stats initialized: Health=" + std::to_string(50.f) +
            ", AttackDamage=" + std::to_string(attackDamage) +
            ", AttackRange=" + std::to_string(attackRange));

        Engine::EventSystem::GetInstance().Subscribe("DamageEvent",
            [this](const Engine::EventsTemp& event) {
                const auto& damageEvent = static_cast<const Engine::DamageEvent&>(event);
                if (damageEvent.GetTarget() == this->gameObject) {
                    auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
                    if (soundManager) {
                        LOG_INFO("Enemy taking damage, playing hit sound");
                        soundManager->PlaySound("Kill");
                    } else {
                        LOG_WARN("Enemy sound manager not found");
                    }

                    auto stats = gameObject->GetComponent<UnitStatsComponent>();
                    if (stats) {
                        float currentHealth = stats->GetHealth();
                        stats->SetHealth(currentHealth - damageEvent.GetDamage());
                        LOG_INFO("Enemy took " + std::to_string(damageEvent.GetDamage()) +
                            " damage. Health: " + std::to_string(stats->GetHealth()));

                        auto effect = gameObject->GetComponent<Engine::EffectComponent>();
                        if (effect) {
                            effect->AddHitEffect(0.2f);
                        }

                        // Смерть
                        if (stats->GetHealth() <= 0) {
                            LOG_INFO("Enemy died!");
                            Engine::GameWorld::Instance()->DestroyGameobject(gameObject);
                        }
                    }
                }
            });
    }

    Enemy::~Enemy()
    {
    }

    void Enemy::Update(float deltaTime)
    {
        if (currentCooldown > 0) {
            currentCooldown -= deltaTime;
        }
        FindAndChasePlayer();
    }

    void Enemy::FindAndChasePlayer()
    {
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        if (!transform)
        {
            LOG_ERROR("Enemy transform component is null!");
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
                //LOG_DEBUG("Player found in detection range");
                break;
            }
        }

        if (player) {
            auto playerTransform = player->GetComponent<Engine::TransformComponent>();
            if (!playerTransform) {
                LOG_WARN("Player transform component not found");
                return;
            }

            Engine::Vector2Df myPos = transform->GetWorldPosition();
            Engine::Vector2Df playerPos = playerTransform->GetWorldPosition();
            Engine::Vector2Df direction = { playerPos.x - myPos.x, playerPos.y - myPos.y };

            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
            if (!rigidbody) {
                LOG_ERROR("Enemy rigidbody component not found");
                return;
            }

            if (distance <= attackRange) {
                rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
                AttackPlayer(player);
            }
            else if (distance > 0) {
                direction.x /= distance;
                direction.y /= distance;

                Engine::Vector2Df velocity = { direction.x * moveSpeed, direction.y * moveSpeed };
                rigidbody->SetLinearVelocity(velocity);
            }
        }
        else {
            auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
            if (rigidbody) {
                rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
            }
        }
    }
    void Enemy::AttackPlayer(Engine::GameObject* player)
    {
        if (currentCooldown <= 0)
        {
            auto damageable = player->GetComponent<Engine::DamageableComponent>();
            if (damageable) {

                auto effect = gameObject->GetComponent<Engine::EffectComponent>();
                if (effect) {
                    effect->AddHitEffect(0.1f); 
                }

                damageable->TakeDamage(attackDamage, gameObject);
                currentCooldown = attackCooldown;
                LOG_INFO("Enemy attacked player for " + std::to_string(attackDamage) + " damage");
            } else {
                LOG_WARN("Player damageable component not found");
              }

        }
        else {
            //LOG_DEBUG("Attack on cooldown: " + std::to_string(currentCooldown));
        }
    }
}