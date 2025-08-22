#pragma once
#include "Components/Physics/Actor/ActorComponent.h"
#include "Systems/Event/EventSystem.h"

namespace Engine
{
	class LevelPointsComponent : public ActorComponent
	{
	public:
		// События уровня
		struct LevelStartEvent : public EventsTemp
		{
			GameObject* startPoint;

			explicit LevelStartEvent(GameObject* point)
				: startPoint(point) {}
			std::string GetType() const override { return "LevelStartEvent"; }
		};

		struct LevelEndEvent : public EventsTemp
		{
			GameObject* endPoint;
			bool levelCompleted;
			explicit LevelEndEvent(GameObject* point, bool completed)
				: endPoint(point), levelCompleted(completed) {}
			std::string GetType() const override { return "LevelEndEvent"; }
		};

		LevelPointsComponent(GameObject* gameObject);
		~LevelPointsComponent() override = default;

		// Установка точек
		void SetStartPoint(GameObject* startPoint);
		void SetEndPoint(GameObject* endPoint);
		void Update(float deltaTime) override;
		void Render() override;
		// Получение точек
		GameObject* GetStartPoint() const;
		GameObject* GetEndPoint() const;

	protected:
		void OnPlayerEnter(GameObject* player) override;

	private:
		bool IsPlayerEnter(GameObject* obj) const;
		GameObject* startPoint = nullptr;
		GameObject* endPoint = nullptr;
		bool isStartPoint = false;
		bool isEndPoint = false;
		int activated = 0;
	};
} // namespace Engine