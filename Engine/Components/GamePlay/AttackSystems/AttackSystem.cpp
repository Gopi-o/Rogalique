#include "pch.h"
#include "AttackSystem.h"
#include <Components/GamePlay/Damageable/DamageableComponent.h>
#include <Core/GameWorld/GameWorld.h>
#include <Systems/Event/EventSystem.h>
#include <Components/GamePlay/Effect/EffectComponent.h>
#include <Components/Sound/SoundManagerComponent.h>

namespace Engine
{
	AttackSystem::AttackSystem(GameObject* gameObject)
		: Component(gameObject) {}

	void AttackSystem::AddAttack(const std::string& name, const AttackParams& params)
	{
		m_attacks[name] = params;
		m_cooldowns[name] = 0.0f;
	}

	void AttackSystem::PerformAttack(const std::string& attackName)
	{
		auto it = m_attacks.find(attackName);
		if (it == m_attacks.end() || m_cooldowns[attackName] > 0)
			return;

		const AttackParams& params = it->second;
		auto transform = gameObject->GetComponent<TransformComponent>();
		if (!transform)
			return;

		// Воспроизведение звука атаки
		auto soundManager = gameObject->GetComponent<SoundManagerComponent>();
		if (soundManager && !params.soundName.empty())
		{
			soundManager->PlaySound(params.soundName);
		}

		// Поиск целей в зависимости от типа атаки
		std::vector<GameObject*> targets;
		switch (params.type)
		{
			case AttackType::Melee:
			case AttackType::AreaOfEffect:
				targets = GameWorld::Instance()->FindObjectsInRadius(
					transform->GetWorldPosition(),
					params.type == AttackType::AreaOfEffect ? params.areaRadius : params.range);
				break;

			case AttackType::Ranged:
				// Реализация для дальних атак (нужен Raycast)
				break;

			case AttackType::Projectile:
				// Создание снаряда
				break;
		}

		// Нанесение урона целям
		for (auto target : targets)
		{
			if (target == gameObject)
				continue;

			bool isValidTarget = false;
			if (gameObject->GetTag() == "Player" && target->GetTag() == "Enemy")
				isValidTarget = true;
			else if (gameObject->GetTag() == "Enemy" && target->GetTag() == "Player")
				isValidTarget = true;

			if (isValidTarget)
			{
				auto damageable = target->GetComponent<DamageableComponent>();
				if (damageable)
				{
					damageable->TakeDamage(params.damage, gameObject);

					// Эффект попадания
					auto effect = target->GetComponent<EffectComponent>();
					if (effect)
					{
						effect->AddHitEffect(0.2f);
					}
				}
			}
		}

		// Эффект атаки на самом атакующем
		auto effect = gameObject->GetComponent<EffectComponent>();
		if (effect)
		{
			effect->AddHitEffect(0.1f);
		}

		m_cooldowns[attackName] = params.cooldown;
	}

	void AttackSystem::Update(float deltaTime)
	{
		for (auto& cooldown : m_cooldowns)
		{
			if (cooldown.second > 0)
				cooldown.second -= deltaTime;
		}
	}
} // namespace Engine