#pragma once

#include <Components/Physics/Transform/TransformComponent.h>
#include <Components/Physics/Rigidbody/RigidbodyComponent.h>
#include <Math/Vector.h>
#include <Components/Render/SpriteRenderer/SpriteRendererComponent.h>
#include <Components/GamePlay/Damageable/DamageableComponent.h>

namespace RogaliqueGame
{
	class Enemy
	{
	public:
		Enemy();
		~Enemy();

		void Update(float deltaTime);
		Engine::GameObject* GetGameObject() const { return gameObject; }
		bool IsAlive() const { return isAlive && gameObject; }
		void UnsubscribeAllEvents();

	private:
		Engine::GameObject* gameObject;
		std::vector<std::string> subscribedEvents;
		bool isAlive = true;
		// Life stats
		float enemyHealth = 100.0f;
		float enemyArmor = 5.0f;

		// Attack stats
		float moveSpeed = 0.35f;
		float detectionRange = 300.0f;
		float attackRange = 50.0f;
		float attackCooldown = 2.6f;
		float attackDamage = 20.0f;

		void FindAndChasePlayer();
		void SetupComponents();
		void LoadResources();
		void SetupAttackSystem();
		void SetupEnemyStats();
		void SetupEventHandlers();
	};
} // namespace RogaliqueGame