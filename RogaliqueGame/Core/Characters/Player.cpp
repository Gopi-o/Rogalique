#include "Player.h"
#include <Systems/Logger.h>
#include <Systems/Resource/ResourceSystem.h>
#include <Components/Sound/SoundManagerComponent.h>
#include <filesystem>
#include <Editor/LevelEditor.h>"
#include "../../Stats/UnitStatsComponent.h"
#include <Components/GamePlay/Effect/EffectComponent.h>
#include <Components/Physics/Actor/A_test/LevelPointsComponent.h>
#include <Components/GamePlay/AttackSystems/AttackSystem.h>
#include <Core/GameStateManager.h>

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
		InitializeComponents();

		// Инициализация статов
		SetupPlayerStats();

		// Подписка на События
		SetupEvents();

		// Инициализация системы атаки
		InitializeAttackSystems();

		// Инициализация ресурсов
		LoadResources();
	}

	void Player::Update(float deltaTime)
	{
		auto input = gameObject->GetComponent<Engine::InputComponent>();
		auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
		auto attackSystem = gameObject->GetComponent<Engine::AttackSystem>();

		if (input && rigidbody && attackSystem)
		{
			float horizontal = input->GetHorizontalAxis();
			float vertical = input->GetVerticalAxis();

			Engine::Vector2Df movement(horizontal * moveSpeed, vertical * moveSpeed);
			rigidbody->SetLinearVelocity(movement);

			if (input->IsAttack())
			{
				attackSystem->PerformAttack("basic");
			}
		}
		else
		{
			LOG_ERROR("Input, Rigidbody or AttackSystem component not found");
		}
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}

	void Player::UnsubscribeAllEvents()
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

	// Метода инициализации
	void Player::InitializeComponents()
	{
		// Основыне физические компоненты
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

		playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);
		playerCamera->SetBaseResolution(1280, 720);

		// Ввод и звук

		// GamePlay
	}
	void Player::SetupInputBind()
	{
	}
	void Player::SetupEvents()
	{
		auto& eventSystem = Engine::EventSystem::GetInstance();

		subscribedEvents.push_back("LevelStartEvent");
		eventSystem.Subscribe("LevelStartEvent",
			[this](const Engine::EventsTemp& event) {
				LOG_INFO("LevelStartEvent received!");
				const auto& startEvent = static_cast<const Engine::LevelPointsComponent::LevelStartEvent&>(event);
				LOG_INFO("Player detected level start point");
			});

		subscribedEvents.push_back("LevelEndEvent");
		eventSystem.Subscribe("LevelEndEvent",
			[this](const Engine::EventsTemp& event) {
				const auto& endEvent = static_cast<const Engine::LevelPointsComponent::LevelEndEvent&>(event);
				if (endEvent.levelCompleted)
				{
					LOG_INFO("Player completed the level!");
					Engine::GameStateManager::Instance()->RestartCurrentScene();
				}
				else
				{
					LOG_WARN("Player reached exit but conditions not met");
				}
			});

		subscribedEvents.push_back("DamageEvent");
		eventSystem.Subscribe("DamageEvent",
			[this](const Engine::EventsTemp& event) {
				const auto& damageEvent = static_cast<const Engine::DamageEvent&>(event);
				if (damageEvent.GetTarget() != this->gameObject)
					return;

				HandleDamage(damageEvent.GetDamage());
			});
	}
	void Player::InitializeAttackSystems()
	{
		auto attackSystem = gameObject->AddComponent<Engine::AttackSystem>();

		// Базовая атака
		Engine::AttackSystem::AttackParams basicAttack;
		basicAttack.type = Engine::AttackSystem::AttackType::Melee;
		basicAttack.damage = attackDamage;
		basicAttack.range = attackRange;
		basicAttack.cooldown = attackCooldown;
		basicAttack.animationName = "attack_melee";
		basicAttack.soundName = "hit";

		attackSystem->AddAttack("basic", basicAttack);
	}
	void Player::SetupPlayerStats()
	{
		auto stats = gameObject->GetComponent<UnitStatsComponent>();
		stats->SetHealth(playerHealth);
		stats->SetArmor(playerArmor);

		LOG_INFO("Player stats - Health: " + std::to_string(stats->GetHealth()) + ", Armor: " + std::to_string(stats->GetArmor()));
		LOG_INFO(std::to_string(stats->GetHealth()));
		LOG_INFO(std::to_string(stats->GetArmor()));
	}
	void Player::LoadResources()
	{
		auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();

		std::string soundPath = "Resources\\Sounds\\swamp_low_quality.wav";
		if (std::filesystem::exists(soundPath))
		{
			LOG_WARN("Sound file does not exist at path: " + soundPath);
		}

		soundManager->AddSound("ambient", "Resources\\Sounds\\swamp_low_quality.wav", 10.0f, true);
		soundManager->AddSound("hit", "Resources\\Sounds\\AppleEat.wav", 50.0f, false);

		soundManager->PlaySound("ambient");
	}
	void Player::HandleDamage(float damage)
	{
		auto soundManager = gameObject->GetComponent<Engine::SoundManagerComponent>();
		if (soundManager)
			soundManager->PlaySound("hit");

		auto stats = gameObject->GetComponent<UnitStatsComponent>();
		if (!stats)
		{
			LOG_ERROR("UnitStats component not found");
			return;
		}

		stats->SetHealth(stats->GetHealth() - damage);
		LOG_INFO("Player took " + std::to_string(damage) + " damage. Health: " + std::to_string(stats->GetHealth()));
		Engine::HealthChangedEvent healthEvent(stats->GetHealth(), playerHealth);
		Engine::EventSystem::GetInstance().Dispatch(healthEvent);

		if (auto effect = gameObject->GetComponent<Engine::EffectComponent>())
		{
			effect->AddHitEffect(0.2f);
		}

		if (stats->GetHealth() <= 0)
		{
			LOG_INFO("Player died!");
			// Обработка смерти игрока
		}
	}
} // namespace RogaliqueGame