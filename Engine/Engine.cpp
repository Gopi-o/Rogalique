#include "pch.h"
#include "Engine.h"
#include <iostream>

namespace Engine
{
	Engine* Engine::Instance()
	{
		static Engine instance;
		return &instance;
	}

	Engine::Engine() :
		window(sf::VideoMode(800, 600), "Engine game")
	{
		// Init random number generator
		unsigned int seed = (unsigned int)time(nullptr); // Get current time as seed. You can also use any other number to fix randomization
		srand(seed);
	}

	void Engine::Run()
	{
		// Init game clock
		sf::Clock gameClock;
		sf::Event event;
		
		// Game loop
		while (window.isOpen()) {

			float startTime = gameClock.getElapsedTime().asSeconds();
			while (window.pollEvent(event))
			{
				// Close window if close button or Escape key pressed
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}

			if (!window.isOpen()) {
				break;
			}

			window.clear();
			window.display();
		}
	}
}