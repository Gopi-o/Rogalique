#include "Player.h"
#include <Systems/Logger.h>
#include <Systems/Resource/ResourceSystem.h>
#include <Components/Sound/SoundManagerComponent.h>
#include <filesystem>
#include <Editor/LevelEditor.h>"
#include "UnitStatsComponent.h"
#include <Components/GamePlay/Effect/EffectComponent.h>
#include <Components/Physics/Actor/A_test/LevelPointsComponent.h>

namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject();
		gameObject->SetTag("Player");
		if (!gameObject)
		{
			LOG_FATAL("Player::Constructor: gameObject is nullptr!");
			return;
		}

		// Добавляем компоненты
		auto transform = gameObject->AddComponent<Engine::TransformComponent>();
		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		auto soundManager = gameObject->AddComponent<Engine::SoundManagerComponent>();
		auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
		auto playerInput = gameObject->AddComponent<Engine::InputComponent>();
		auto collidery = gameObject->AddComponent<Engine::SpriteColliderComponent>();
		auto UnitStats = gameObject->AddComponent<UnitStatsComponent>();
		auto damageable = gameObject->AddComponent<Engine::DamageableComponent>();
		auto effectComponent = gameObject->AddComponent<Engine::EffectComponent>();

		std::string soundPath = "Resources\\Sounds\\swamp_low_quality.wav";
		if (std::filesystem::exists(soundPath))
		{
			LOG_WARN("Sound file does not exist at path: " + soundPath);

			/*std::cout << "Sound file does not exist at path: " << soundPath << std::endl;
			std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
			return;*/
		}

		soundManager->AddSound("ambient", "Resources\\Sounds\\swamp_low_quality.wav", 10.0f, true);
		soundManager->AddSound("hit", "Resources\\Sounds\\AppleEat.wav", 50.0f, false);

		soundManager->PlaySound("ambient");

		UnitStats->SetHealth(550.f);
		UnitStats->SetArmor(20.f);
		attackRange = 100.0f;
		attackCooldown = 2.6f;
		currentCooldown = 0.0f;
		attackDamage = 20.0f;

		LOG_INFO(std::to_string(UnitStats->GetHealth()));
		LOG_INFO(std::to_string(UnitStats->GetArmor()));

		auto& eventSystem = Engine::EventSystem::GetInstance();
		
		eventSystem.Subscribe("LevelStartEvent",
			[this](const Engine::EventsTemp& event) {
				LOG_INFO("LevelStartEvent received!");
				const auto& startEvent = static_cast<const Engine::LevelPointsComponent::LevelStartEvent&>(event);
				LOG_INFO("Player detected level start point");
			});
		

		Engine::EventSystem::GetInstance().Subscribe("LevelEndEvent",
			[this](const Engine::EventsTemp& event) {
				const auto& endEvent = static_cast<const Engine::LevelPointsComponent::LevelEndEvent&>(event);
				if (endEvent.levelCompleted)
				{
					LOG_INFO("Player completed the level!");
				}
				else
				{
					LOG_WARN("Player reached exit but conditions not met");
				}
			});

		Engine::EventSystem::GetInstance().Subscribe("DamageEvent",
			[this](const Engine::EventsTemp& event) {
				const auto& damageEvent = static_cast<const Engine::DamageEvent&>(event);
				if (damageEvent.GetTarget() == this->gameObject)
				{

					auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
					if (soundManager)
					{
						LOG_INFO("SoundManager found, attempting to play hit sound");
						soundManager->PlaySound("hit");
					}
					else
					{
						LOG_ERROR("SoundManager not found!");
					}

					auto stats = gameObject->GetComponent<UnitStatsComponent>();
					if (stats)
					{
						float currentHealth = stats->GetHealth();
						stats->SetHealth(currentHealth - damageEvent.GetDamage());
						LOG_INFO("Player took " + std::to_string(damageEvent.GetDamage()) + " damage. Health: " + std::to_string(stats->GetHealth()));

						auto effect = gameObject->GetComponent<Engine::EffectComponent>();
						if (effect)
						{
							effect->AddHitEffect(0.2f);
						}

						// смерть
						if (stats->GetHealth() <= 0)
						{
							LOG_INFO("Player died!");
						}
					}
					else
					{
						LOG_ERROR("UnitStats component not found");
					}
				}
			});

		playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		playerCamera->SetBaseResolution(1280, 720);

		moveSpeed = 2.0f;
	}

	void Player::Update(float deltaTime)
	{
		if (currentCooldown > 0)
		{
			currentCooldown -= deltaTime;
		}

		auto input = gameObject->GetComponent<Engine::InputComponent>();
		auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();

		if (input && rigidbody)
		{
			float horizontal = input->GetHorizontalAxis();
			float vertical = input->GetVerticalAxis();

			Engine::Vector2Df movement(horizontal * moveSpeed, vertical * moveSpeed);

			rigidbody->SetLinearVelocity(movement);

			if (input->IsAttack() && currentCooldown <= 0)
			{
				Attack();
			}
		}
		else
		{
			LOG_ERROR("Input or Rigidbody component not found");
		}
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}

	void Player::Attack()
	{
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		if (!transform)
			return;

		auto objectsInRange = Engine::GameWorld::Instance()->FindObjectsInRadius(
			transform->GetWorldPosition(),
			attackRange);

		for (auto obj : objectsInRange)
		{
			if (obj->GetTag() == "Enemy")
			{
				auto damageable = obj->GetComponent<Engine::DamageableComponent>();
				if (damageable)
				{
					damageable->TakeDamage(attackDamage, gameObject);

					auto effect = obj->GetComponent<Engine::EffectComponent>();
					if (effect)
					{
						effect->AddHitEffect(0.2f);
					}
				}
			}
		}

		auto effect = gameObject->GetComponent<Engine::EffectComponent>();
		if (effect)
		{
			effect->AddHitEffect(0.1f);
		}

		auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
		if (soundManager)
		{
			soundManager->PlaySound("hit");
		}

		currentCooldown = attackCooldown;
	}
} // namespace RogaliqueGame