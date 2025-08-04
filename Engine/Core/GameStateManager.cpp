#include "pch.h"
#include "GameStateManager.h"


namespace Engine
{
	void GameStateManager::RegisterScene(const std::string& name, std::unique_ptr<Scene> scene)
	{
		scenes[name] = std::move(scene);
	}

	bool GameStateManager::SwitchToScene(const std::string& name)
	{
		if (isSwitchingScenes)
			return false;
		isSwitchingScenes = true;

		auto it = scenes.find(name);
		if (it == scenes.end())
			return false;

		if (currentScene)
			currentScene->Stop();

		currentScene = it->second.get();
		currentSceneName = name;
		currentScene->Start();

		isSwitchingScenes = false;
		return true;
	}

	void GameStateManager::RestartCurrentScene()
	{
		if (currentScene)
			currentScene->Restart();
	}

	void GameStateManager::UpdateCurrentScene(float deltaTime)
	{
		if (currentScene)
			currentScene->Update(deltaTime);
	}

	void GameStateManager::HandleEvent(const sf::Event& event)
	{
		if (currentScene)
		{
			currentScene->HandleEvent(event);
		}
	}

	void GameStateManager::Clear()
	{
		if (currentScene)
		{
			currentScene->Stop();
			currentScene = nullptr;
		}
		scenes.clear();
	}
} // namespace Engine