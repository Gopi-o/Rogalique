#include "UnitStatsComponent.h"


namespace RogaliqueGame
{
	UnitStatsComponent::UnitStatsComponent(Engine::GameObject* gameObject)
		: Engine::StatsComponent(gameObject)
	{
		SetHealth(100.f);
		SetArmor(0.0f);
		SetLevel(1);
		SetStatus("normal");
		SetAlive(true);
	}

	void UnitStatsComponent::SetHealth(float value)
	{
		SetValue<float>(KEY_HEALTH, value);

		if (value <= 0)
		{
			SetAlive(false);
		}
	}

	float UnitStatsComponent::GetHealth() const
	{
		return GetValue<float>(KEY_HEALTH);
	}

	void UnitStatsComponent::ModifyHealth(float amount)
	{
		float currentHealth = GetHealth();
		SetHealth(currentHealth + amount);
	}

	void UnitStatsComponent::SetArmor(float value)
	{
		SetValue<float>(KEY_ARMOR, value);
	}

	float UnitStatsComponent::GetArmor() const
	{
		return GetValue<float>(KEY_ARMOR);
	}

	void UnitStatsComponent::ModifyArmor(float amount)
	{
		float currnetArmor = GetArmor();
		SetArmor(currnetArmor + amount);
	}

	void UnitStatsComponent::SetLevel(int value)
	{
		SetValue<float>(KEY_LEVEL, value);
	}

	int UnitStatsComponent::GetLevel() const
	{
		return GetValue<int>(KEY_LEVEL);
	}

	void UnitStatsComponent::ModifyLevel(int value)
	{
		SetValue<int>(KEY_LEVEL, value);
	}

	void UnitStatsComponent::SetStatus(const std::string& value)
	{
		SetValue<std::string>(KEY_STATUS, value);
	}

	std::string UnitStatsComponent::GetStatus() const
	{
		return GetValue<std::string>(KEY_STATUS);
	}

	bool UnitStatsComponent::isAlive(bool value)
	{
		return GetValue<bool>(KEY_ALIVE);
	}

	void UnitStatsComponent::SetAlive(bool value)
	{
		SetValue<bool>(KEY_ALIVE, value);
	}

}