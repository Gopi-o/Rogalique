#include "pch.h"
#include "Enemy.h"
#include <Core/GameWorld/GameWorld.h>
#include <Systems/Resource/ResourceSystem.h>
#include <Editor/LevelEditor.h>
#include <Systems/Logger.h>
#include <Components/GamePlay/Effect/EffectComponent.h>
#include "../../Stats/UnitStatsComponent.h"
#include <Components/Sound/SoundManagerComponent.h>
#include <Components/GamePlay/AttackSystems/AttackSystem.h>
#include <filesystem>

namespace RogaliqueGame
{
	Enemy::Enemy()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject();
		gameObject->SetTag("Enemy");
		LOG_INFO("Enemy created");

		SetupComponents();
		SetupEnemyStats();
		LoadResources();
		SetupAttackSystem();
		SetupEventHandlers();
	}

	Enemy::~Enemy()
	{
	}

	void Enemy::SetupComponents()
	{
		auto transform = gameObject->AddComponent<Engine::TransformComponent>();
		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
		auto damageable = gameObject->AddComponent<Engine::DamageableComponent>();
		auto stats = gameObject->AddComponent<UnitStatsComponent>();
		auto effectComponent = gameObject->AddComponent<Engine::EffectComponent>();
		auto soundManager = gameObject->AddComponent<Engine::SoundManagerComponent>();

		renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("enemy"));
		renderer->SetPixelSize(40, 40);
	}
	void Enemy::LoadResources()
	{
		auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
		soundManager->AddSound("kill", "Resources\\Sounds\\Kill.wav", 50.0f, false);
	}
	void Enemy::SetupAttackSystem()
	{
		auto attackSystem = gameObject->AddComponent<Engine::AttackSystem>();

		Engine::AttackSystem::AttackParams basicAttack;
		basicAttack.type = Engine::AttackSystem::AttackType::Melee;
		basicAttack.damage = attackDamage;
		basicAttack.range = attackRange;
		basicAttack.cooldown = attackCooldown;
		basicAttack.animationName = "attack_melee";
		basicAttack.soundName = "hit";

		attackSystem->AddAttack("basic", basicAttack);
	}

	void Enemy::SetupEnemyStats()
	{
		auto stats = gameObject->GetComponent<UnitStatsComponent>();
		stats->SetHealth(enemyHealth);
		stats->SetArmor(enemyArmor);

		LOG_INFO("Enemy stats - Health: " + std::to_string(stats->GetHealth()) + ", Armor: " + std::to_string(stats->GetArmor()));
		LOG_INFO(std::to_string(stats->GetHealth()));
		LOG_INFO(std::to_string(stats->GetArmor()));
	}

	void Enemy::SetupEventHandlers()
	{
		Engine::EventSystem::GetInstance().Subscribe("DamageEvent",
			[this](const Engine::EventsTemp& event) {
				const auto& damageEvent = static_cast<const Engine::DamageEvent&>(event);
				if (damageEvent.GetTarget() == this->gameObject)
				{
					auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
					if (soundManager)
					{
						LOG_INFO("Enemy taking damage, playing hit sound");
						soundManager->PlaySound("kill");
					}

					auto stats = gameObject->GetComponent<UnitStatsComponent>();
					if (stats)
					{
						float currentHealth = stats->GetHealth();
						stats->SetHealth(currentHealth - damageEvent.GetDamage());
						LOG_INFO("Enemy took " + std::to_string(damageEvent.GetDamage()) + " damage. Health: " + std::to_string(stats->GetHealth()));

						auto effect = gameObject->GetComponent<Engine::EffectComponent>();
						if (effect)
						{
							effect->AddHitEffect(0.2f);
						}

						if (stats->GetHealth() <= 0)
						{
							LOG_INFO("Enemy died!");
							Engine::GameWorld::Instance()->DestroyGameobject(gameObject);
						}
					}
				}
			});
	}

	void Enemy::Update(float deltaTime)
	{
		FindAndChasePlayer();
	}

	void Enemy::FindAndChasePlayer()
	{
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		if (!transform)
			return;

		auto objectsInRange = Engine::GameWorld::Instance()->FindObjectsInRadius(
			transform->GetWorldPosition(),
			detectionRange);

		Engine::GameObject* player = nullptr;
		for (auto obj : objectsInRange)
		{
			if (obj->GetTag() == "Player")
			{
				player = obj;
				break;
			}
		}

		if (!player)
		{
			auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
			if (rigidbody)
				rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
			return;
		}

		auto playerTransform = player->GetComponent<Engine::TransformComponent>();
		if (!playerTransform)
			return;

		Engine::Vector2Df myPos = transform->GetWorldPosition();
		Engine::Vector2Df playerPos = playerTransform->GetWorldPosition();
		Engine::Vector2Df direction = { playerPos.x - myPos.x, playerPos.y - myPos.y };

		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
		if (!rigidbody)
			return;

		if (distance > 0)
		{
			direction.x /= distance;
			direction.y /= distance;
			rigidbody->SetLinearVelocity({ direction.x * moveSpeed, direction.y * moveSpeed });
		}

		auto attackSystem = gameObject->GetComponent<Engine::AttackSystem>();
		if (attackSystem && distance <= attackRange)
		{
			rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
			attackSystem->PerformAttack("basic");
		}
	}
} // namespace RogaliqueGame