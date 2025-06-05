#pragma once
#define NOMINMAX

#include "SFML/Graphics.hpp"

namespace Engine
{
	class Engine
	{
	public:
		Engine(const Engine& app) = delete;
		Engine& operator= (const Engine&) = delete;

		static Engine* Instance();

		void Run();

	private:
		Engine();
		~Engine() = default;

	private:
		sf::RenderWindow window;
	};
}