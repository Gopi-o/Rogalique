#include "pch.h"
#include "LevelEditor.h"
#include "Core/GameWorld/GameWorld.h"
#include "Systems/Resource/ResourceSystem.h"
#include "Systems/Render/RenderSystem.h"
#include <Systems/FileMaster/FileMaster.h>
#include <Systems/Logger.h>
#include <fstream>
#include <sstream>

namespace Engine
{
	LevelEditor* LevelEditor::Instance()
	{
		static LevelEditor levelEditor;
		return &levelEditor;
	}

	void LevelEditor::Update(float deltaTime)
	{
		if (isGridVisible)
		{
			DrawGrid();
		}
	}

	void LevelEditor::Render()
	{
	}

	GameObject* LevelEditor::CreateWall(const Vector2Df& position, float width, float height)
	{
		auto wallObject = GameWorld::Instance()->CreateGameObject();
		wallObject->SetTag("wall");

		auto transform = wallObject->GetComponent<TransformComponent>();
		if (!transform)
		{
			std::cout << "Failed to get TransformComponent!" << std::endl;
			return nullptr;
		}

		transform->SetWorldPosition(SnapToGrid(position));
		transform->SetWorldScale(width / gridSize, height / gridSize);

		auto renderer = wallObject->AddComponent<SpriteRendererComponent>();
		if (!renderer)
			return nullptr;

		renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("wall"));
		renderer->SetPixelSize(gridSize, gridSize);

		auto wallComponent = wallObject->AddComponent<WallComponent>();
		if (!wallComponent)
			return nullptr;

		return wallObject;
	}

	GameObject* LevelEditor::CreateFloor(const Vector2Df& position, float width, float height)
	{
		auto floorObject = GameWorld::Instance()->CreateGameObject();
		floorObject->SetTag("Floor");

		auto transform = floorObject->GetComponent<TransformComponent>();
		if (!transform)
		{
			std::cout << "Failed to get TransformComponent!" << std::endl;
			return nullptr;
		}

		transform->SetWorldPosition(SnapToGrid(position));
		transform->SetWorldScale(width / gridSize, height / gridSize);

		auto renderer = floorObject->AddComponent<SpriteRendererComponent>();
		if (!renderer)
			return nullptr;

		renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("floor"));
		renderer->SetPixelSize(gridSize, gridSize);

		auto floorComponent = floorObject->AddComponent<FloorComponent>();
		if (!floorComponent)
			return nullptr;

		return floorObject;
	}

	GameObject* LevelEditor::CreatePointMarker(const Vector2Df& position, const std::string& type)
	{
		GameObject* point = GameWorld::Instance()->CreateGameObject();
		point->SetTag(type);

		auto transform = point->GetComponent<TransformComponent>();
		if (!transform)
		{
			LOG_ERROR("transform point is error!");
		}
		transform->SetWorldPosition(position);

		// Добавляем спрайт (можно использовать разные для старта и выхода)
		auto sprite = point->AddComponent<SpriteRendererComponent>();
		sprite->SetTexture(*ResourceSystem::Instance()->GetTextureShared(type == "StartPoint" ? "start" : "exit"));
		sprite->SetPixelSize(15.0f, 15.0f);

		// Добавляем коллайдер
		auto collider = point->AddComponent<SpriteColliderComponent>();
		collider->SetTrigger(true);

		auto rb = point->AddComponent<RigidbodyComponent>();
		rb->SetKinematic(true); // Как у StaticCollider
		rb->SetLinearDamping(1.0f);

		return point;
	}

	void LevelEditor::SetGridSize(float size)
	{
		gridSize = size;
	}

	void LevelEditor::SetGridVisible(bool visible)
	{
		isGridVisible = visible;
	}

	Vector2Df LevelEditor::SnapToGrid(const Vector2Df& position) const
	{
		return {
			std::round(position.x / gridSize) * gridSize,
			std::round(position.y / gridSize) * gridSize
		};
	}

	void LevelEditor::DrawGrid()
	{
		sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();

		sf::RectangleShape line;
		line.setFillColor(sf::Color(50, 50, 50, 100));

		for (float x = 0; x < window.getSize().x; x += gridSize)
		{
			line.setSize(sf::Vector2f(1, window.getSize().y));
			line.setPosition(x, 0);
			window.draw(line);
		}

		for (float y = 0; y < window.getSize().y; y += gridSize)
		{
			line.setSize(sf::Vector2f(window.getSize().x, 1));
			line.setPosition(0, y);
			window.draw(line);
		}
	}

	void LevelEditor::SaveLevel(const std::string& levelName)
	{
		LevelData data;
		const std::vector<GameObject*>& objects = GameWorld::Instance()->GetAllObjects();

		for (GameObject* obj : objects)
		{
			if (obj->GetTag() == "Wall" || obj->GetTag() == "Floor")
			{
				LevelData::ObjectData objectData;
				objectData.type = obj->GetTag();

				auto transform = obj->GetComponent<TransformComponent>();
				if (transform)
				{
					objectData.position = transform->GetWorldPosition();
					objectData.size = transform->GetWorldScale() * gridSize;
				}

				auto renderer = obj->GetComponent<SpriteRendererComponent>();
				if (renderer)
				{
				}

				data.objects.push_back(objectData);
			}
		}

		SaveToFile("Levels/" + levelName + ".txt", data);
	}

	void LevelEditor::LoadLevel(const std::string& levelName)
	{
		LevelData data = LoadFromFile("Levels/" + levelName + ".json");

		for (const auto& objData : data.objects)
		{
			if (objData.type == "Wall")
			{
				CreateWall(objData.position, objData.size.x, objData.size.y);
			}
			else if (objData.type == "Floor")
			{
				CreateFloor(objData.position, objData.size.x, objData.size.y);
			}
		}
	}

	void LevelEditor::SaveToFile(const std::string& filename, const LevelData& data)
	{
		std::stringstream ss;
		ss << data.objects.size() << "\n";

		for (const auto& obj : data.objects)
		{
			ss << obj.type << "\n";
			ss << obj.position.x << " " << obj.position.y << "\n";
			ss << obj.size.x << " " << obj.size.y << "\n";
			ss << obj.textureName << "\n";
		}

		if (!FileMaster::Instance()->SaveText("Levels/" + filename + "levelName.txt", ss.str()))
		{
			std::cout << "Failed to save level: " << filename << std::endl;
		}
	}

	LevelEditor::LevelData LevelEditor::LoadFromFile(const std::string& filename)
	{
		std::string fileContent;
		if (!FileMaster::Instance()->LoadText("Levels/" + filename + ".txt", fileContent))
		{
			std::cout << "Failed to load level: " << filename << std::endl;
		}

		std::stringstream ss(fileContent);
		LevelData data;
		size_t objectCount;
		ss >> objectCount;
		std::string line;
		std::getline(ss, line);

		for (size_t i = 0; i < objectCount; ++i)
		{
			LevelData::ObjectData objData;

			std::getline(ss, objData.type);

			ss >> objData.position.x >> objData.position.y;
			std::getline(ss, line);

			ss >> objData.size.x >> objData.size.y;
			std::getline(ss, line);

			std::getline(ss, objData.textureName);

			data.objects.push_back(objData);
		}
		return data;
	}
} // namespace Engine