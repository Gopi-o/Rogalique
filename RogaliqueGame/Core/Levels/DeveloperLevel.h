#pragma once
#include <iostream>
#include <array>
#include <Core/Scene.h>
#include "../Characters/Player.h"
#include "../Characters/Enemy.h"
#include <Editor/LevelEditor.h>
#include "../UI/GameHUD.h"

using namespace Engine;

namespace RogaliqueGame
{
	class DeveloperLevel : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;
		void Update(float deltaTime) override;
		void HandleEvent(const sf::Event& event) override;

		void Pause();
		void UnPause();

		void CreateEnemy();
		void CreateLevel();
		void SaveCurrentLevel();
		void LoadLevel(const std::string& levelName);

	private:
		std::shared_ptr<Player> player;
		std::vector<std::shared_ptr<Enemy>> enemies;
		std::shared_ptr<GameHUD> hud;
		bool bIsPaused;

		void UpdateEnemiesCount();
	};
} // namespace RogaliqueGame
