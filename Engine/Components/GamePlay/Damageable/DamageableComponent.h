#pragma once
#include "Components/Component.h"
#include <Systems/Event/EventSystem.h>

namespace Engine
{
	class DamageableComponent : public Component
	{
	public:
		DamageableComponent(GameObject* gameObject);
		virtual ~DamageableComponent();

		void Update(float deltaTime) override;
		void Render() override;

		virtual void TakeDamage(float damage, GameObject* source = nullptr);

		void SetInvincible(bool invincible) { isInvincible = invincible; }
		bool IsInvincible() const { return isInvincible; }

		void SetDamageMultiplier(float multi) { damageMultiplier = multi; }
		float SetDamageMultiplier() { return damageMultiplier; }

	private:
		bool isInvincible = false;
		float damageMultiplier = 1.0f;
	};
} // namespace Engine
