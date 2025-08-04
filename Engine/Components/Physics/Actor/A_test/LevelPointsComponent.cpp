#include "pch.h"
#include "LevelPointsComponent.h"
#include <Components/Physics/Collider/ColliderComponent.h>
#include <Systems/Logger.h>
#include <Core/GameStateManager.h>

namespace Engine
{
	LevelPointsComponent::LevelPointsComponent(GameObject* gameObject)
		: ActorComponent(gameObject), activated(0)
	{
		// По умолчанию активируем обработку входа в триггер
		EnableInteraction(InteractionType::OnTriggerEnter, true);

		// Определяем тип точки (начало или конец уровня)
		auto collider = gameObject->GetComponent<ColliderComponent>();

		collider->SetTrigger(true);

		// Проверяем теги для автоматического определения типа
		const std::string& tag = gameObject->GetTag();
		m_isStartPoint = (tag == "StartPoint");
		m_isEndPoint = (tag == "EndPoint");

		LOG_INFO("Level point created: " + tag);
	}

	void LevelPointsComponent::SetStartPoint(GameObject* startPoint)
	{
		m_startPoint = startPoint;
		m_isStartPoint = (gameObject == startPoint);
	}

	void LevelPointsComponent::SetEndPoint(GameObject* endPoint)
	{
		m_endPoint = endPoint;
		m_isEndPoint = (gameObject == endPoint);
	}

	void LevelPointsComponent::Update(float deltaTime)
	{
		ActorComponent::Update(deltaTime);
	}

	void LevelPointsComponent::Render()
	{
		ActorComponent::Render();
	}

	GameObject* LevelPointsComponent::GetStartPoint() const
	{
		return m_startPoint;
	}

	GameObject* LevelPointsComponent::GetEndPoint() const
	{
		return m_endPoint;
	}

	void LevelPointsComponent::OnPlayerEnter(GameObject* player)
	{
		if (!player)
			return;

		LOG_DEBUG("Object entered point: " + player->GetTag());

		if (!IsPlayerEnter(player))
		{
			LOG_DEBUG("Not a player object: " + player->GetTag());
			return;
		}

		auto transform = gameObject->GetComponent<TransformComponent>();
		if (!transform)
			return;

		Vector2Df pos = transform->GetWorldPosition();

		if (m_isStartPoint)
		{
			LOG_INFO("PLAYER ENTERED START POINT at (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")");

			EventSystem::GetInstance().Dispatch(LevelStartEvent(gameObject));
		}
		else if (m_isEndPoint)
		{
			bool completed = false;
			if (activated >= 1)
			{
				completed = true; // Здесь можно добавить логику проверки
				EventSystem::GetInstance().Dispatch(LevelEndEvent(gameObject, completed));
				// Engine::GameStateManager::Instance()->RestartCurrentScene();
			}
			activated += 1;

			LOG_INFO("PLAYER ENTERED EXIT POINT at (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")");
		}
	}
	bool LevelPointsComponent::IsPlayerEnter(GameObject* obj) const
	{
		return obj && obj->GetTag() == "Player";
	}
} // namespace Engine