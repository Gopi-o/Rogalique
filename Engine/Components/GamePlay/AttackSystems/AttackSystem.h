#pragma once
#include <Components/Component.h>
#include <Math/Vector.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine
{
	class AttackSystem : public Component
	{
	public:
		enum class AttackType
		{
			Melee,
			Ranged,
			AreaOfEffect,
			Projectile
		};

		struct AttackParams
		{
			AttackType type;
			float damage;
			float range;
			float cooldown;
			float areaRadius;
			std::string animationName;
			std::string soundName;
		};

		AttackSystem(GameObject* gameObject);

		void AddAttack(const std::string& name, const AttackParams& params);
		void PerformAttack(const std::string& attackName);
		void Update(float deltaTime) override;
		void Render() override {}

	private:
		std::unordered_map<std::string, AttackParams> m_attacks;
		std::unordered_map<std::string, float> m_cooldowns;
	};
} // namespace Engine