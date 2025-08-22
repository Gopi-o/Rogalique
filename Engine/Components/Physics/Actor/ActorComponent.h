#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Components/Component.h"
#include "Components/Physics/Collision/Collision.h"
#include "Systems/Trigger/Trigger.h"
#include "Systems/Event/EventSystem.h"
#include "Components/Physics/Transform/TransformComponent.h"

namespace Engine
{
	class ActorComponent : public Component
	{
	public:
		enum class InteractionType
		{
			OnTriggerEnter,
			OnTriggerExit,
			OnInteract
		};

		ActorComponent(GameObject* gameObject);
		virtual ~ActorComponent();

		void Update(float deltaTime) override;
		void Render() override;

		void Activate();
		void Deactivate();
		bool IsActive() const;

		void EnableInteraction(InteractionType type, bool enable);
		bool IsInteractionEnabled(InteractionType type) const;

		void SubscribeOnInteraction(InteractionType type, std::function<void(GameObject*)> callback);
		void UnsubscribeOnInteraction(InteractionType type, std::function<void(GameObject*)> callback);

		void SetVisualRepresentation(sf::Sprite* sprite);
		sf::Sprite* GetVisualRepresentation() const;

	protected:
		virtual void OnActivated() {}
		virtual void OnDeactivated() {}
		virtual void OnInteraction(GameObject* interactor) {}
		virtual void OnPlayerEnter(GameObject* player) {}
		virtual void OnPlayerExit(GameObject* player) {}

	private:
		void HandleCollision(Collision collision);
		void HandleTriggerEnter(Trigger trigger);
		void HandleTriggerExit(Trigger trigger);

		GameObject* GetOtherFromTrigger(GameObject* current, Trigger trigger) const;
		Vector2Df GetPosition() const;

		bool isActive;
		sf::Sprite* visualRepresentation;
		bool interactOnTriggerEnter;
		bool interactOnTriggerExit;
		bool interactOnCommand;

		std::vector<std::function<void(GameObject*)>> onInteractCallbacks;
		std::vector<std::function<void(GameObject*)>> onEnterCallbacks;
		std::vector<std::function<void(GameObject*)>> onExitCallbacks;

		std::function<void(Collision)> collisionHandler;
		std::function<void(Trigger)> triggerEnterHandler;
		std::function<void(Trigger)> triggerExitHandler;
	};
} // namespace Engine