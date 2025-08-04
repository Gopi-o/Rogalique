#pragma once
#include <Components/Render/Camera/CameraComponent.h>
#include <Core/GameWorld/GameWorld.h>
#include <Components/Render/SpriteRenderer/SpriteRendererComponent.h>
#include <Systems/Render/RenderSystem.h>
#include <Components/GamePlay/Input/InputComponent.h>
#include <Core/GameObject/GameObject.h>
#include <Components/GamePlay/Damageable/DamageableComponent.h>

namespace RogaliqueGame
{
	class Player
	{
	public:
		Player();
		void Update(float deltaTime);
		Engine::GameObject* GetGameObject();

		float GetMaxHealth() const { return playerMaxHealth; }
		void UnsubscribeAllEvents();

	private:
		void InitializeComponents();
		void SetupInputBind();
		void SetupEvents();
		void InitializeAttackSystems();
		void SetupPlayerStats();
		void LoadResources();
		void HandleDamage(float damage);

		Engine::GameObject* gameObject;
		std::vector<std::string> subscribedEvents;

		// Life stats
		float playerHealth = 50.0f;
		float playerMaxHealth = playerHealth;
		float playerArmor = 20.0f;

		// Attack stats
		float moveSpeed = 5.4f;
		float attackRange = 100.0f;
		float attackCooldown = 2.6f;
		float attackDamage = 20.0f;
	};
} // namespace RogaliqueGame
