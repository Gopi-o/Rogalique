#include "pch.h"
#include "ActorComponent.h"
#include "Systems/Physics/PhysicsSystem.h"
#include <Systems/Render/RenderSystem.h>
#include <Systems/Logger.h>

namespace Engine
{
	ActorComponent::ActorComponent(GameObject* gameObject)
		: Component(gameObject), m_isActive(true), m_visualRepresentation(nullptr), m_interactOnTriggerEnter(false), m_interactOnTriggerExit(false), m_interactOnCommand(false)
	{
		m_collisionHandler = [this](Collision collision) { HandleCollision(collision); };
		m_triggerEnterHandler = [this](Trigger trigger) {
			HandleTriggerEnter(trigger);
		};
		m_triggerExitHandler = [this](Trigger trigger) { HandleTriggerExit(trigger); };

		auto collider = gameObject->GetComponent<ColliderComponent>();
		if (collider)
		{
			collider->SubscribeCollision(m_collisionHandler);
			collider->SubscribeTriggerEnter(m_triggerEnterHandler);
			collider->SubscribeTriggerExit(m_triggerExitHandler);
		}
	}

	ActorComponent::~ActorComponent()
	{
		auto collider = gameObject->GetComponent<ColliderComponent>();
		if (collider)
		{
			collider->UnsubscribeCollision(m_collisionHandler);
			collider->UnsubscribeTriggerEnter(m_triggerEnterHandler);
			collider->UnsubscribeTriggerExit(m_triggerExitHandler);
		}
	}

	void ActorComponent::Update(float deltaTime)
	{
		m_triggerEnterHandler = [this](Trigger trigger) {
			HandleTriggerEnter(trigger);
		};
		if (!m_isActive)
		{
			LOG_DEBUG("ActorComponent skipped update - not active");
			return;
		}

		auto transform = gameObject->GetComponent<TransformComponent>();
		if (transform)
		{
			auto pos = transform->GetWorldPosition();
			if (m_visualRepresentation) // Опционально, если нужен только для рендеринга
				m_visualRepresentation->setPosition(pos.x, pos.y);
		}
		else
		{
			LOG_WARN("ActorComponent has no TransformComponent");
		}
	}

	void ActorComponent::Render()
	{
		if (!m_isActive || !m_visualRepresentation)
			return;
		RenderSystem::Instance()->Render(*m_visualRepresentation);
	}

	void ActorComponent::Activate()
	{
		if (m_isActive)
			return;
		m_isActive = true;
		OnActivated();
	}

	void ActorComponent::Deactivate()
	{
		if (!m_isActive)
			return;
		m_isActive = false;
		OnDeactivated();
	}

	bool ActorComponent::IsActive() const
	{
		return m_isActive;
	}

	void ActorComponent::EnableInteraction(InteractionType type, bool enable)
	{
		switch (type)
		{
			case InteractionType::OnTriggerEnter:
				m_interactOnTriggerEnter = enable;
				break;
			case InteractionType::OnTriggerExit:
				m_interactOnTriggerExit = enable;
				break;
			case InteractionType::OnInteract:
				m_interactOnCommand = enable;
				break;
		}
	}

	bool ActorComponent::IsInteractionEnabled(InteractionType type) const
	{
		switch (type)
		{
			case InteractionType::OnTriggerEnter:
				return m_interactOnTriggerEnter;
			case InteractionType::OnTriggerExit:
				return m_interactOnTriggerExit;
			case InteractionType::OnInteract:
				return m_interactOnCommand;
			default:
				return false;
		}
	}

	void ActorComponent::SubscribeOnInteraction(InteractionType type, std::function<void(GameObject*)> callback)
	{
		switch (type)
		{
			case InteractionType::OnTriggerEnter:
				m_onEnterCallbacks.push_back(callback);
				break;
			case InteractionType::OnTriggerExit:
				m_onExitCallbacks.push_back(callback);
				break;
			case InteractionType::OnInteract:
				m_onInteractCallbacks.push_back(callback);
				break;
		}
	}

	void ActorComponent::UnsubscribeOnInteraction(InteractionType type, std::function<void(GameObject*)> callback)
	{
		auto& callbacks =
			(type == InteractionType::OnTriggerEnter) ? m_onEnterCallbacks : (type == InteractionType::OnTriggerExit) ? m_onExitCallbacks
																													  : m_onInteractCallbacks;

		callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
							[&callback](const std::function<void(GameObject*)>& cb) {
								return cb.target_type() == callback.target_type();
							}),
			callbacks.end());
	}

	void ActorComponent::SetVisualRepresentation(sf::Sprite* sprite)
	{
		m_visualRepresentation = sprite;
		if (m_visualRepresentation)
		{
			auto transform = gameObject->GetComponent<TransformComponent>();
			if (transform)
			{
				m_visualRepresentation->setPosition(transform->GetWorldPosition().x,
					transform->GetWorldPosition().y);
			}
		}
	}

	sf::Sprite* ActorComponent::GetVisualRepresentation() const
	{
		return m_visualRepresentation;
	}

	void ActorComponent::HandleCollision(Collision collision)
	{
		if (!m_isActive)
			return;
		// Базовая обработка коллизий
	}

	void ActorComponent::HandleTriggerEnter(Trigger trigger)
	{
		LOG_DEBUG("TriggerEnter detected between: " + std::to_string(reinterpret_cast<uintptr_t>(trigger.GetFirst())) + " and " + std::to_string(reinterpret_cast<uintptr_t>(trigger.GetSecond())));

		if (!m_isActive || !m_interactOnTriggerEnter)
			return;

		GameObject* other = GetOtherFromTrigger(gameObject, trigger);
		if (!other)
		{
			LOG_DEBUG("Other object not found in trigger");
			return;
		}

		LOG_DEBUG("Trigger with object: " + other->GetTag());
		OnPlayerEnter(other);

		for (auto& callback : m_onEnterCallbacks)
		{
			callback(other);
		}
	}

	void ActorComponent::HandleTriggerExit(Trigger trigger)
	{
		if (!m_isActive || !m_interactOnTriggerExit)
			return;

		GameObject* other = GetOtherFromTrigger(gameObject, trigger);
		if (!other)
			return;

		OnPlayerExit(other);

		for (auto& callback : m_onExitCallbacks)
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