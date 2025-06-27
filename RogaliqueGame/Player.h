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

	private:
		Engine::GameObject* gameObject;
		float moveSpeed;
		float attackRange;
		float attackCooldown;
		float currentCooldown;
		float attackDamage;

		void Attack();
	};
} // namespace RogaliqueGame
