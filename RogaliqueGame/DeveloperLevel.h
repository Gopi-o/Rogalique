#pragma once
#include <iostream>
#include <array>
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "LevelEditor.h"

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

		void CreateEnemy();
		void CreateLevel(); 
		void SaveCurrentLevel(); 
		void LoadLevel(const std::string& levelName);

	private:
		std::shared_ptr<Player> player;
		std::vector<std::shared_ptr<Enemy>> enemies;

	};
}

