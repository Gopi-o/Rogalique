#pragma once
#include <SFML/Window/Event.hpp>

namespace Engine
{
	class Scene
	{
	public:
		virtual void Start() = 0;
		virtual void Restart() = 0;
		virtual void Stop() = 0;
		virtual void Update(float deltaTime) = 0;

		virtual void HandleEvent(const sf::Event& event) {}
	};
}

