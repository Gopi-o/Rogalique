#include "pch.h"
#include "ActorComponent.h"
#include "Systems/Physics/PhysicsSystem.h"
#include <Systems/Render/RenderSystem.h>
#include <Systems/Logger.h>

namespace Engine
{
	ActorComponent::ActorComponent(GameObject* gameObject)
		: Component(gameObject), isActive(true), visualRepresentation(nullptr), interactOnTriggerEnter(false), interactOnTriggerExit(false), interactOnCommand(false)
	{
		collisionHandler = [this](Collision collision) { HandleCollision(collision); };
		triggerEnterHandler = [this](Trigger trigger) {
			HandleTriggerEnter(trigger);
		};
		triggerExitHandler = [this](Trigger trigger) { HandleTriggerExit(trigger); };

		auto collider = gameObject->GetComponent<ColliderComponent>();
		if (collider)
		{
			collider->SubscribeCollision(collisionHandler);
			collider->SubscribeTriggerEnter(triggerEnterHandler);
			collider->SubscribeTriggerExit(triggerExitHandler);
		}
	}

	ActorComponent::~ActorComponent()
	{
		if (gameObject == nullptr)
			return;

		auto collider = gameObject->GetComponent<ColliderComponent>();
		if (!collider)
			return;
		if (!collider->GetGameObject())
			return;
		collider->UnsubscribeCollision(collisionHandler);
		collider->UnsubscribeTriggerEnter(triggerEnterHandler);
		collider->UnsubscribeTriggerExit(triggerExitHandler);
	}

	void ActorComponent::Update(float deltaTime)
	{
		/*m_triggerEnterHandler = [this](Trigger trigger) {
			HandleTriggerEnter(trigger);
		};*/
		if (!isActive)
		{
			LOG_DEBUG("ActorComponent skipped update - not active");
			return;
		}

		auto transform = gameObject->GetComponent<TransformComponent>();
		if (transform)
		{
			auto pos = transform->GetWorldPosition();
			if (visualRepresentation) // �����������, ���� ����� ������ ��� ����������
				visualRepresentation->setPosition(pos.x, pos.y);
		}
		else
		{
			LOG_WARN("ActorComponent has no TransformComponent");
		}
	}

	void ActorComponent::Render()
	{
		if (!isActive || !visualRepresentation)
			return;
		RenderSystem::Instance()->Render(*visualRepresentation);
	}

	void ActorComponent::Activate()
	{
		if (isActive)
			return;
		isActive = true;
		OnActivated();
	}

	void ActorComponent::Deactivate()
	{
		if (!isActive)
			return;
		isActive = false;
		OnDeactivated();
	}

	bool ActorComponent::IsActive() const
	{
		return isActive;
	}

	void ActorComponent::EnableInteraction(InteractionType type, bool enable)
	{
		switch (type)
		{
			case InteractionType::OnTriggerEnter:
				interactOnTriggerEnter = enable;
				break;
			case InteractionType::OnTriggerExit:
				interactOnTriggerExit = enable;
				break;
			case InteractionType::OnInteract:
				interactOnCommand = enable;
				break;
		}
	}

	bool ActorComponent::IsInteractionEnabled(InteractionType type) const
	{
		switch (type)
		{
			case InteractionType::OnTriggerEnter:
				return interactOnTriggerEnter;
			case InteractionType::OnTriggerExit:
				return interactOnTriggerExit;
			case InteractionType::OnInteract:
				return interactOnCommand;
			default:
				return false;
		}
	}

	void ActorComponent::SubscribeOnInteraction(InteractionType type, std::function<void(GameObject*)> callback)
	{
		switch (type)
		{
			case InteractionType::OnTriggerEnter:
				onEnterCallbacks.push_back(callback);
				break;
			case InteractionType::OnTriggerExit:
				onExitCallbacks.push_back(callback);
				break;
			case InteractionType::OnInteract:
				onInteractCallbacks.push_back(callback);
				break;
		}
	}

	void ActorComponent::UnsubscribeOnInteraction(InteractionType type, std::function<void(GameObject*)> callback)
	{
		auto& callbacks =
			(type == InteractionType::OnTriggerEnter) ? onEnterCallbacks : (type == InteractionType::OnTriggerExit) ? onExitCallbacks
																													: onInteractCallbacks;

		callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
							[&callback](const std::function<void(GameObject*)>& cb) {
								return cb.target_type() == callback.target_type();
							}),
			callbacks.end());
	}

	void ActorComponent::SetVisualRepresentation(sf::Sprite* sprite)
	{
		visualRepresentation = sprite;
		if (visualRepresentation)
		{
			auto transform = gameObject->GetComponent<TransformComponent>();
			if (transform)
			{
				visualRepresentation->setPosition(transform->GetWorldPosition().x,
					transform->GetWorldPosition().y);
			}
		}
	}

	sf::Sprite* ActorComponent::GetVisualRepresentation() const
	{
		return visualRepresentation;
	}

	void ActorComponent::HandleCollision(Collision collision)
	{
		if (!isActive)
			return;
		// ������� ��������� ��������
	}

	void ActorComponent::HandleTriggerEnter(Trigger trigger)
	{
		LOG_DEBUG("TriggerEnter detected between: " + std::to_string(reinterpret_cast<uintptr_t>(trigger.GetFirst())) + " and " + std::to_string(reinterpret_cast<uintptr_t>(trigger.GetSecond())));

		if (!isActive || !interactOnTriggerEnter)
			return;

		GameObject* other = GetOtherFromTrigger(gameObject, trigger);
		if (!other)
		{
			LOG_DEBUG("Other object not found in trigger");
			return;
		}

		LOG_DEBUG("Trigger with object: " + other->GetTag());
		OnPlayerEnter(other);

		for (auto& callback : onEnterCallbacks)
		{
			callback(other);
		}
	}

	void ActorComponent::HandleTriggerExit(Trigger trigger)
	{
		if (!isActive || !interactOnTriggerExit)
			return;

		GameObject* other = GetOtherFromTrigger(gameObject, trigger);
		if (!other)
			return;

		OnPlayerExit(other);

		for (auto& callback : onExitCallbacks)
		{
			callback(other);
		}
	}

	GameObject* ActorComponent::GetOtherFromTrigger(GameObject* current, Trigger trigger) const
	{
		auto firstCollider = trigger.GetFirst();
		auto secondCollider = trigger.GetSecond();

		if (firstCollider && firstCollider->GetGameObject() == current)
		{
			return secondCollider ? secondCollider->GetGameObject() : nullptr;
		}
		return firstCollider ? firstCollider->GetGameObject() : nullptr;
	}

	Vector2Df ActorComponent::GetPosition() const
	{
		auto transform = gameObject->GetComponent<TransformComponent>();
		return transform ? transform->GetWorldPosition() : Vector2Df();
	}
} // namespace Engine