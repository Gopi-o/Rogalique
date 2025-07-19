#include "DeveloperLevel.h"
#include <Systems/Resource/ResourceSystem.h>
#include <filesystem>
#include <Systems/Logger.h>
#include <Components/Render/MazeGeneratorComponent.h>
#include "../../Stats/UnitStatsComponent.h"

using namespace Engine;

namespace RogaliqueGame
{

	void DeveloperLevel::Start()
	{
		bIsPaused = false;
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

		hud = std::make_shared<GameHUD>();
		if (auto stats = plObject->GetComponent<UnitStatsComponent>())
		{
			hud->SetHealth(static_cast<int>(stats->GetHealth()),
				static_cast<int>(player->GetMaxHealth()));
		}
		hud->SetAmmo(30, 120);
		UpdateEnemiesCount();
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
		player->UnsubscribeAllEvents();
		for (auto& enemy : enemies)
		{
			if (enemy)
			{
				enemy->UnsubscribeAllEvents();
			}
		}
		hud.reset();
		enemies.clear();
		GameWorld::Instance()->Clear();
		player.reset();
	}

	void DeveloperLevel::Update(float deltaTime)
	{
		if (hud)
		{
			hud->Update(deltaTime);
		}

		if (auto input = player->GetGameObject()->GetComponent<InputComponent>())
		{
			if (input->IsPause())
			{
				// Переключаем состояние паузы
				if (hud)
					hud->SetPaused(!hud->IsPaused());
				bIsPaused = !bIsPaused;
			}
		}

		if (!bIsPaused)
		{
			if (player)
			{
				player->Update(deltaTime);
			}

			UpdateEnemiesCount();

			for (auto& enemy : enemies)
			{
				enemy->Update(deltaTime);
			}
		}
	}

	void DeveloperLevel::HandleEvent(const sf::Event& event)
	{
		if (hud)
		{
			hud->HandleEvent(event);
		}
	}

	void DeveloperLevel::Pause()
	{
		bIsPaused = true;
	}

	void DeveloperLevel::UnPause()
	{
		bIsPaused = false;
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

	void DeveloperLevel::UpdateEnemiesCount()
	{
		if (!hud)
			return;

		// Удаляем уничтоженных врагов из списка
		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
						  [](const std::shared_ptr<Enemy>& enemy) {
							  return !enemy || !enemy->IsAlive();
						  }),
			enemies.end());

		// Обновляем счетчик в HUD
		hud->SetEnemiesCount(static_cast<int>(enemies.size()));
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