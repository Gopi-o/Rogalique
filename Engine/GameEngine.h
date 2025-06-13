#pragma once
#define NOMINMAX

#include "SFML/Graphics.hpp"

namespace Engine
{
	class GameEngine
	{
	public:
		GameEngine(const GameEngine& app) = delete;
		GameEngine& operator= (const GameEngine&) = delete;

		static GameEngine* Instance();

		void Run();

	private:
		GameEngine();
		~GameEngine() = default;
	};
}