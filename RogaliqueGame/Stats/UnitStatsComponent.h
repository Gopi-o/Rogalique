#pragma once
#include <Components/GamePlay/Stats/StatsComponent.h>

namespace RogaliqueGame
{
	class UnitStatsComponent : public Engine::StatsComponent
	{
	public:
		UnitStatsComponent(Engine::GameObject* gameObject);

		void SetHealth(float value);
		float GetHealth() const;
		void ModifyHealth(float amount);

		void SetArmor(float value);
		float GetArmor() const;
		void ModifyArmor(float amount);

		void SetLevel(int value);
		int GetLevel() const;
		void ModifyLevel(int value);

		void SetStatus(const std::string& value);
		std::string GetStatus() const;

		bool isAlive(bool value);
		void SetAlive(bool value);

	private:
		static constexpr const char* KEY_HEALTH = "health";
		static constexpr const char* KEY_MAX_HEALTH = "maxHealth";
		static constexpr const char* KEY_ARMOR = "armor";
		static constexpr const char* KEY_LEVEL = "level";
		static constexpr const char* KEY_STATUS = "status";
		static constexpr const char* KEY_ALIVE = "isAlive";
	};
} // namespace RogaliqueGame
