#pragma once
#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "DamageableComponent.h"



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
}


