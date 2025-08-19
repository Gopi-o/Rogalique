#include "pch.h"
#include "ColliderComponent.h"
#include <Systems/Logger.h>

namespace Engine
{
	ColliderComponent::ColliderComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	void ColliderComponent::Update(float deltaTime)
	{
	}

	void ColliderComponent::SetTrigger(bool newIsTrigger)
	{
		isTrigger = newIsTrigger;
	}

	void ColliderComponent::SubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		onCollisionActions.push_back(onCollisionAction);
	}
	void ColliderComponent::UnsubscribeCollision(std::function<void(Collision)> onCollisionAction)
	{
		onCollisionActions.erase(std::remove_if(
									 onCollisionActions.begin(),
									 onCollisionActions.end(),
									 [&onCollisionAction](const std::function<void(Collision)>& action) {
										 return action.target<void(Collision)>() == onCollisionAction.target<void(Collision)>();
									 }),
			onCollisionActions.end());
	}

	void ColliderComponent::SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		onTriggerEnterActions.push_back(onTriggerEnterAction);
	}
	void ColliderComponent::UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
	{
		onTriggerEnterActions.erase(std::remove_if(
										onTriggerEnterActions.begin(),
										onTriggerEnterActions.end(),
										[&onTriggerEnterAction](const std::function<void(Trigger)>& action) {
											return action.target<void(Trigger)>() == onTriggerEnterAction.target<void(Trigger)>();
										}),
			onTriggerEnterActions.end());
	}

	void ColliderComponent::SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		onTriggerExitActions.push_back(onTriggerExitAction);
	}
	void ColliderComponent::UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
	{
		onTriggerExitActions.erase(std::remove_if(
									   onTriggerExitActions.begin(),
									   onTriggerExitActions.end(),
									   [&onTriggerExitAction](const std::function<void(Trigger)>& action) {
										   return action.target<void(Trigger)>() == onTriggerExitAction.target<void(Trigger)>();
									   }),
			onTriggerExitActions.end());
	}

	void ColliderComponent::OnCollision(Collision collision)
	{

		auto actions = onCollisionActions;
		for (auto& fn : actions)
		{
			if (fn)
				fn(collision);
		}
	}
	void ColliderComponent::OnTriggerEnter(Trigger trigger)
	{

		auto actions = onTriggerEnterActions;
		for (auto& fn : actions)
		{
			if (fn)
				fn(trigger);
		}
	}
	void ColliderComponent::OnTriggerExit(Trigger trigger)
	{
		auto actions = onTriggerExitActions;
		for (auto& fn : actions)
		{
			if (fn)
				fn(trigger);
		}
	}
} // namespace Engine