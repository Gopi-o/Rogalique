#include "DeveloperLevel.h"
#include <Systems/Resource/ResourceSystem.h>
#include <filesystem>
#include <Systems/Logger.h>
#include <Components/Render/MazeGeneratorComponent.h>

using namespace Engine;

namespace RogaliqueGame
{

	void DeveloperLevel::Start()
	{
		std::string soundPath = "Resources/Textures/wall.jpg";

		ResourceSystem::Instance()->LoadTexture("wall", "Resources/Textures/wall.jpg");
		ResourceSystem::Instance()->LoadTexture("floor", "Resources/Textures/floor.png");
		ResourceSystem::Instance()->LoadTexture("start", "Resources/Textures/start.png");
		ResourceSystem::Instance()->LoadTexture("exit", "Resources/Textures/exit.png");
		ResourceSystem::Instance()->LoadTexture("health_icon", "Resources/Textures/BodyBend.png");

		CreateLevel();

		player = std::make_shared<Player>();
		auto plObject = player->GetGameObject();

		auto mazeGeneratorObj = GameWorld::Instance()->FindObjectByTag("MazeGenerator");
		if (mazeGeneratorObj)
		{
			auto mazeGenerator = mazeGeneratorObj->GetComponent<MazeGeneratorComponent>();
			if (mazeGenerator)
			{
				auto startPoint = mazeGenerator->GetStartPointPos();
				LOG_INFO("player Pos after create level (" + std::to_string(startPoint.x) + ", " + std::to_string(startPoint.y) + ")");
				auto transform = plObject->GetComponent<TransformComponent>();
				if (transform)
				{
					transform->SetWorldPosition(startPoint.x, startPoint.y);
				}

				auto spawnPoints = mazeGenerator->GetEnemySpawnPoints();
				for (const auto& spawnPos : spawnPoints)
				{
					auto enemy = std::make_shared<Enemy>();
					auto enemyObj = enemy->GetGameObject();
					auto enemyTransform = enemyObj->GetComponent<TransformComponent>();
					if (enemyTransform)
					{
						enemyTransform->SetWorldPosition(spawnPos);
						enemies.push_back(enemy);
					}
				}
			}
		}

		auto hud = std::make_shared<GameHUD>();
		hud->SetHealth(100, 100);
		hud->SetAmmo(30, 120);
		hud->SetEnemiesCount(static_cast<int>(enemies.size()));
		hud->SetLevelInfo("developer");
		// CreateEnemy();
	}
	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}
	void DeveloperLevel::Stop()
	{
		GameWorld::Instance()->Clear();
	}

	void DeveloperLevel::Update(float deltaTime)
	{
		if (player)
		{
			player->Update(deltaTime);
		}

		if (hud)
		{
			hud->Update(deltaTime);
		}

		for (auto& enemy : enemies)
		{
			enemy->Update(deltaTime);
		}
	}

	void DeveloperLevel::CreateLevel()
	{
		auto mazeGeneratorObj = GameWorld::Instance()->CreateGameObject();
		mazeGeneratorObj->SetTag("MazeGenerator");
		auto mazeGenerator = mazeGeneratorObj->AddComponent<MazeGeneratorComponent>();

		mazeGenerator->Generate(5, 5);
	}

	void DeveloperLevel::SaveCurrentLevel()
	{

		std::filesystem::create_directories("Levels");
		LevelEditor::Instance()->SaveLevel("developer_level");
	}

	void DeveloperLevel::LoadLevel(const std::string& levelName)
	{
		Stop();

		LevelEditor::Instance()->LoadLevel(levelName);

		player = std::make_shared<Player>();
		CreateEnemy();
	}

	void DeveloperLevel::CreateEnemy()
	{
		auto enemy = std::make_shared<Enemy>();
		auto enemyObject = enemy->GetGameObject();

		auto transform = enemyObject->GetComponent<TransformComponent>();
		if (transform)
		{
			transform->SetWorldPosition(100, 0);
		}

		enemies.push_back(enemy);
	}
} // namespace RogaliqueGame