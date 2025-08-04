#include "pch.h"
#include "GameEngine.h"
#include <iostream>
#include "Core/GameWorld/GameWorld.h"
#include <Systems/Render/RenderSystem.h>
#include <Systems/Physics/PhysicsSystem.h>
#include <Core/GameStateManager.h>

namespace Engine
{
	GameEngine* GameEngine::Instance()
	{
		static GameEngine instance;
		return &instance;
	}

	GameEngine::GameEngine()
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);
	}

	void GameEngine::Run()
	{
		sf::Clock gameClock;
		sf::Event event;

		const float FIXED_TIMESTEP = 1.0f / 60.0f;
		float accumulator = 0.0f;

		while (RenderSystem::Instance()->GetMainWindow().isOpen())
		{
			while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					RenderSystem::Instance()->GetMainWindow().close();

				GameStateManager::Instance()->HandleEvent(event);
				if (currentScene)
					currentScene->HandleEvent(event);
			}

			if (!RenderSystem::Instance()->GetMainWindow().isOpen())
				break;

			float deltaTime = gameClock.restart().asSeconds();
			accumulator += deltaTime;

			while (accumulator >= FIXED_TIMESTEP)
			{
				GameStateManager::Instance()->UpdateCurrentScene(FIXED_TIMESTEP);
				GameWorld::Instance()->Update(FIXED_TIMESTEP);
				PhysicsSystem::Instance()->Update();

				accumulator -= FIXED_TIMESTEP;
			}

			RenderSystem::Instance()->GetMainWindow().clear();

			GameWorld::Instance()->Render();
			GameWorld::Instance()->LateUpdate();

			RenderSystem::Instance()->GetMainWindow().display();
		}
	}
} // namespace Engine