#include "pch.h"
#include "GameEngine.h"
#include <iostream>
#include "GameWorld.h"
#include "RenderSystem.h"

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

		while (RenderSystem::Instance()->GetMainWindow().isOpen())
		{
			sf::Time dt = gameClock.restart();
			float deltaTime = dt.asSeconds();

			while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					RenderSystem::Instance()->GetMainWindow().close();
				}
			}

			if (!RenderSystem::Instance()->GetMainWindow().isOpen())
			{
				break;
			}

			RenderSystem::Instance()->GetMainWindow().clear();

			GameWorld::Instance()->Update(deltaTime);
			GameWorld::Instance()->Render();
			GameWorld::Instance()->LateUpdate();

			RenderSystem::Instance()->GetMainWindow().display();
		}
	}
}