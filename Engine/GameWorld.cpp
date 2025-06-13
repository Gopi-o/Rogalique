#include "pch.h"
#include "GameWorld.h"

namespace Engine
{
	GameWorld* GameWorld::Instance()
	{
		static GameWorld world;
		return &world;
	}

	void GameWorld::Update(float deltaTime)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Update(deltaTime);
		}
	}

	void GameWorld::Render()
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Render();
		}
	}


	void GameWorld::LateUpdate()
	{
		for (int i = markedToDestroyGameObject.size() - 1; i >= 0; i--)
		{
			DestroyGameObjectImmediative(markedToDestroyGameObject[i]);
		}
	}

	GameObject* GameWorld::CreateGameObject()
	{
		GameObject* newGameObject = new GameObject();
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}

	void GameWorld::DestroyGameobject(GameObject* gameObject)
	{
		markedToDestroyGameObject.push_back(gameObject);
	}

	void GameWorld::Clear()
	{
		for (int i = gameObjects.size() -1; i >= 0; i--)
		{
			DestroyGameObjectImmediative(gameObjects[i]);
		}
	}

	GameObject* GameWorld::FindObjectByName(const std::string& name)
	{
		for (auto obj : gameObjects)
		{
			if (obj->GetName() == name) {
				return obj;
			}
		}
		return nullptr;
	}

	

	GameObject* GameWorld::FindClosestObject(const Vector2Df& position, float maxDistance)
	{
		GameObject* closest = nullptr;
		float minDistance = maxDistance;

		// »щем среди активных объектов
		for (auto obj : gameObjects)
		{
			auto transform = obj->GetComponent<TransformComponent>();
			if (!transform) continue;

			Vector2Df objPos = transform->GetWorldPosition();
			float distance = std::sqrt(
				(position.x - objPos.x) * (position.x - objPos.x) +
				(position.y - objPos.y) * (position.y - objPos.y)
			);

			if (maxDistance < 0 || distance <= maxDistance)
			{
				if (!closest || distance < minDistance)
				{
					closest = obj;
					minDistance = distance;
				}
			}
		}

		return closest;
	}

	std::vector<GameObject*> GameWorld::FindObjectsInRadius(const Vector2Df& position, float radius)
	{
		std::vector<GameObject*> result;
		float radiusSquared = radius * radius;

		// »щем среди активных объектов
		for (auto obj : gameObjects)
		{
			auto transform = obj->GetComponent<TransformComponent>();
			if (!transform) continue;

			Vector2Df objPos = transform->GetWorldPosition();
			float distanceSquared =
				(position.x - objPos.x) * (position.x - objPos.x) +
				(position.y - objPos.y) * (position.y - objPos.y);

			if (distanceSquared <= radiusSquared)
			{
				result.push_back(obj);
			}
		}

		return result;
	}

	void GameWorld::DestroyGameObjectImmediative(GameObject* gameObject)
	{
		gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [gameObject](GameObject* obj) { return obj == gameObject; }), gameObjects.end());
		markedToDestroyGameObject.erase(std::remove_if(markedToDestroyGameObject.begin(), markedToDestroyGameObject.end(), [gameObject](GameObject* obj) { return obj == gameObject; }), markedToDestroyGameObject.end());

		delete gameObject;
	}
}