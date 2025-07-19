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
		// Create enemy game object and set basic properties
		gameObject = Engine::GameWorld::Instance()->CreateGameObject();
		gameObject->SetTag("Enemy");
		LOG_INFO("Enemy created");

		// Initialize enemy components and systems
		SetupComponents();
		SetupEnemyStats();
		LoadResources();
		SetupAttackSystem();
		SetupEventHandlers();
	}

	Enemy::~Enemy()
	{
	}

	// Sets up all required components for the enemy
	void Enemy::SetupComponents()
	{
		// Add and configure standard components
		auto transform = gameObject->AddComponent<Engine::TransformComponent>();
		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
		auto damageable = gameObject->AddComponent<Engine::DamageableComponent>();
		auto stats = gameObject->AddComponent<UnitStatsComponent>();
		auto effectComponent = gameObject->AddComponent<Engine::EffectComponent>();
		auto soundManager = gameObject->AddComponent<Engine::SoundManagerComponent>();

		// Set enemy appearance
		renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("enemy"));
		renderer->SetPixelSize(16, 16);
	}

	// Loads sound effects for the enemy
	void Enemy::LoadResources()
	{
		auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
		soundManager->AddSound("kill", "Resources\\Sounds\\Kill.wav", 50.0f, false);
	}

	// Configures the enemy's attack system
	void Enemy::SetupAttackSystem()
	{
		auto attackSystem = gameObject->AddComponent<Engine::AttackSystem>();

		// Set up basic melee attack
		Engine::AttackSystem::AttackParams basicAttack;
		basicAttack.type = Engine::AttackSystem::AttackType::Melee;
		basicAttack.damage = attackDamage;
		basicAttack.range = attackRange;
		basicAttack.cooldown = attackCooldown;
		basicAttack.animationName = "attack_melee";
		basicAttack.soundName = "hit";

		attackSystem->AddAttack("basic", basicAttack);
	}

	// Set initial stats for the enemy
	void Enemy::SetupEnemyStats()
	{
		auto stats = gameObject->GetComponent<UnitStatsComponent>();
		stats->SetHealth(enemyHealth);
		stats->SetArmor(enemyArmor);

		// Log initial stats for debugging
		LOG_INFO("Enemy stats - Health: " + std::to_string(stats->GetHealth()) + ", Armor: " + std::to_string(stats->GetArmor()));
		LOG_INFO(std::to_string(stats->GetHealth()));
		LOG_INFO(std::to_string(stats->GetArmor()));
	}

	// Set eventы for damage and other events
	void Enemy::SetupEventHandlers()
	{
		subscribedEvents.push_back("DamageEvent");
		Engine::EventSystem::GetInstance().Subscribe("DamageEvent",
			[this](const Engine::EventsTemp& event) {
				if (!this || !gameObject)
					return;

				// Handle damage event
				const auto& damageEvent = static_cast<const Engine::DamageEvent&>(event);
				if (damageEvent.GetTarget() == this->gameObject)
				{

					// Play hit sound when damaged
					auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
					if (soundManager)
					{
						LOG_INFO("Enemy taking damage, playing hit sound");
						soundManager->PlaySound("kill");
					}

					// Update health and check for death
					auto stats = gameObject->GetComponent<UnitStatsComponent>();
					if (stats)
					{
						float currentHealth = stats->GetHealth();
						stats->SetHealth(currentHealth - damageEvent.GetDamage());
						LOG_INFO("Enemy took " + std::to_string(damageEvent.GetDamage()) + " damage. Health: " + std::to_string(stats->GetHealth()));

						// Show hit effect
						auto effect = gameObject->GetComponent<Engine::EffectComponent>();
						if (effect)
						{
							effect->AddHitEffect(0.2f);
						}

						// Handle death
						if (stats->GetHealth() <= 0)
						{
							LOG_INFO("Enemy died!");
							isAlive = false;
							Engine::GameWorld::Instance()->DestroyGameobject(gameObject);
						}
					}
				}
			});
	}

	// Main update function called every frame
	void Enemy::Update(float deltaTime)
	{
		if (!gameObject)
			return;
		FindAndChasePlayer();
	}

	// Clean up event subscriptions
	void Enemy::UnsubscribeAllEvents()
	{
		auto& eventSystem = Engine::EventSystem::GetInstance();
		Engine::EventSystem::EventCallback emptyCallback;
		// Отписываемся от всех событий, на которые подписывались
		for (const auto& eventName : subscribedEvents)
		{
			eventSystem.Unsubscribe(eventName, emptyCallback);
		}
		subscribedEvents.clear();
	}

	// find and chase player
	void Enemy::FindAndChasePlayer()
	{
		if (!this)
			return;
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();

		// Find all objects near the enemy
		auto objectsInRange = Engine::GameWorld::Instance()->FindObjectsInRadius(
			transform->GetWorldPosition(),
			detectionRange);

		// Look for player in nearby objects
		Engine::GameObject* player = nullptr;
		for (auto obj : objectsInRange)
		{
			if (obj->GetTag() == "Player")
			{
				player = obj;
				break;
			}
		}

		// Stop moving if no player found
		if (!player)
		{
			auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
			if (rigidbody)
				rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
			return;
		}

		// Move toward player if found
		auto playerTransform = player->GetComponent<Engine::TransformComponent>();
		if (!playerTransform)
			return;

		Engine::Vector2Df myPos = transform->GetWorldPosition();
		Engine::Vector2Df playerPos = playerTransform->GetWorldPosition();
		Engine::Vector2Df direction = { playerPos.x - myPos.x, playerPos.y - myPos.y };

		// Normalize direction and move
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
		if (!rigidbody)
			return;

		if (distance > 0 && player->GetComponent<UnitStatsComponent>()->GetHealth() >= 0)
		{
			direction.x /= distance;
			direction.y /= distance;
			rigidbody->SetLinearVelocity({ direction.x * moveSpeed, direction.y * moveSpeed });
		}

		// Attack if player is close enough
		auto attackSystem = gameObject->GetComponent<Engine::AttackSystem>();
		if (attackSystem && distance <= attackRange && player->GetComponent<UnitStatsComponent>()->GetHealth() >= 0)
		{
			rigidbody->SetLinearVelocity(Engine::Vector2Df{ 0, 0 });
			attackSystem->PerformAttack("basic");
		}
	}
} // namespace RogaliqueGame