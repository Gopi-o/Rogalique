#pragma once
#define NOMINMAX

#include "SFML/Graphics.hpp"
#include "Scene.h"

namespace Engine
{
	class GameEngine
	{
	public:
		GameEngine(const GameEngine& app) = delete;
		GameEngine& operator= (const GameEngine&) = delete;

		static GameEngine* Instance();

		void Run();
		void SetCurrentScene(Scene* scene) { currentScene = scene; }

	private:
		GameEngine();
		~GameEngine() = default;

		Scene* currentScene = nullptr;

	};
}