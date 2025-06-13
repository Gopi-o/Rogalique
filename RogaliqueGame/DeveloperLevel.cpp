#include "DeveloperLevel.h"
#include "Enemy.h"
#include "ResourceSystem.h"
#include <filesystem>


using namespace Engine;

namespace RogaliqueGame
{

	void DeveloperLevel::Start()
	{
		std::string soundPath = "Resources/Textures/wall.jpg";
		

		ResourceSystem::Instance()->LoadTexture("wall", "Resources/Textures/wall.jpg");
		ResourceSystem::Instance()->LoadTexture("floor", "Resources/Textures/floor.png");


		CreateLevel();

		player = std::make_shared<Player>();
		CreateEnemy();
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
		for (auto& enemy : enemies)
		{
			enemy->Update(deltaTime);
		}
	}

	void DeveloperLevel::CreateLevel()
	{
		LevelEditor::Instance()->CreateWall({ 0, 350 }, 1280, 32); // Верхняя стена
		LevelEditor::Instance()->CreateWall({ 0, -350 }, 1280, 32); // Нижняя стена
		LevelEditor::Instance()->CreateWall({ -632, 0 }, 32, 720); // Левая стена
		LevelEditor::Instance()->CreateWall({ 632, 0 }, 32, 720); // Правая стена

		// Пол
		LevelEditor::Instance()->CreateFloor({ 0, 0 }, 1240, 650);

		// Создаем дополнительные стены для создания комнат
		LevelEditor::Instance()->CreateWall({ 400, 200 }, 32, 300); // Вертикальная стена
		LevelEditor::Instance()->CreateWall({ 400, 200 }, 400, 32); // Горизонтальная стена
	}

	void DeveloperLevel::SaveCurrentLevel()
	{
		std::filesystem::create_directories("Levels");

		std::filesystem::create_directories("Levels");
		LevelEditor::Instance()->SaveLevel("developer_level");;
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
}