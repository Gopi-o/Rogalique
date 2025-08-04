#include "pch.h"
#include "DamageableComponent.h"
#include <Systems/Event/EventsTemp.h>

namespace Engine
{
	DamageableComponent::DamageableComponent(GameObject* gameObject)
		: Component(gameObject) {}

	DamageableComponent::~DamageableComponent() {}

	void DamageableComponent::Update(float deltaTime) {}

	void DamageableComponent::Render() {}

	void DamageableComponent::TakeDamage(float damage, GameObject* source)
	{
		if (isInvincible)
			return;

		float finalDamage = damage * damageMultiplier;
		EventSystem::GetInstance().Dispatch(DamageEvent(gameObject, finalDamage, source));
	}

} // namespace Engine