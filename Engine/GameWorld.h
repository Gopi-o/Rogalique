#pragma once
#include "GameObject.h"

namespace Engine
{
	class GameWorld
	{
	public:
		static GameWorld* Instance();

		void Update(float deltaTime);
		void Render();
		void LateUpdate();

		GameObject* CreateGameObject();
		void DestroyGameobject(GameObject* gameObject);
		void Clear();

		 GameObject* FindObjectByName(const std::string& name);
        GameObject* FindClosestObject(const Vector2Df& position, float maxDistance = -1.0f);
        std::vector<GameObject*> FindObjectsInRadius(const Vector2Df& position, float radius);

		const std::vector<GameObject*>& GetAllObjects() const { return gameObjects; }

	private:
		GameWorld() {}
		~GameWorld() {}

		GameWorld(GameWorld const&) = delete;
		GameWorld& operator= (GameWorld const&) = delete;

		std::vector<GameObject*> gameObjects = {};
		std::vector<GameObject*> markedToDestroyGameObject = {};

		void DestroyGameObjectImmediative(GameObject* gameObject);
	};
}

