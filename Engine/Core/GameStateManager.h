#pragma once
#include <Core/Scene.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Window/Window.hpp>

namespace Engine
{
	class GameStateManager
	{
	public:
		static GameStateManager* Instance()
		{
			static GameStateManager instance;
			return &instance;
		}

		void RegisterScene(const std::string& name, std::unique_ptr<Scene> scene);
		bool SwitchToScene(const std::string& name);
		void RestartCurrentScene();

		Scene* GetCurrentScene() const { return currentScene; }
		const std::string& GetCurrentSceneName() const { return currentSceneName; }

		void UpdateCurrentScene(float deltaTime);
		void Clear();

		void HandleEvent(const sf::Event& event);


	private:
		GameStateManager() = default;
		~GameStateManager() = default;
		bool isSwitchingScenes = false;

		std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
		Scene* currentScene = nullptr;
		std::string currentSceneName;
	};
}
