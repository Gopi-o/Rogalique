#pragma once
#include "Core/GameObject/GameObject.h"

namespace Engine
{
	class EventsTemp
	{
	public:
		virtual ~EventsTemp() = default;
		virtual std::string GetType() const = 0;
	};

	class DamageEvent : public EventsTemp
	{
	public:
		DamageEvent(GameObject* target, float damage, GameObject* source = nullptr)
			: target(target), damage(damage), source(source) {}

		std::string GetType() const override { return "DamageEvent"; }

		GameObject* GetTarget() const { return target; }
		float GetDamage() const { return damage; }
		GameObject* GetSource() const { return source; }

	private:
		GameObject* target;
		float damage;
		GameObject* source;
	};

	class DeathEvent : public EventsTemp
	{
	public:
		DeathEvent(GameObject* victim, GameObject* killer = nullptr)
			: victim(victim), killer(killer)
		{
		}

		std::string GetType() const override { return "DeathEvent"; }

		GameObject* GetVictim() const { return victim; }
		GameObject* GetKiller() const { return killer; }

	private:
		GameObject* victim;
		GameObject* killer;
	};

	class LevelStartEvent : public EventsTemp
	{
	public:
		LevelStartEvent(GameObject* levelStartObject)
			: levelStartObject(levelStartObject) {}
		std::string GetType() const override { return "LevelStartEvent"; }

		GameObject* GetLevelStartObject() const { return levelStartObject; }

	private:
		GameObject* levelStartObject;
	};

	class HealthChangedEvent : public EventsTemp
	{
	public:
		HealthChangedEvent(float current, float max)
			: currentHealth(current), maxHealth(max) {}

		std::string GetType() const override { return "HealthChangedEvent"; }

		float currentHealth;
		float maxHealth;
	};

} // namespace Engine
