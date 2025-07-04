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
} // namespace RogaliqueGame