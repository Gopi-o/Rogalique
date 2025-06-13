#include "pch.h"
#include "LevelEditor.h"
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "RenderSystem.h"
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
        // Здесь можно добавить отрисовку UI редактора
    }

    GameObject* LevelEditor::CreateWall(const Vector2Df& position, float width, float height)
    {
        auto wallObject = GameWorld::Instance()->CreateGameObject();
        wallObject->SetTag("Wall");

        auto transform = wallObject->GetComponent<TransformComponent>();
        if (!transform)
        {
            std::cout << "Failed to get TransformComponent!" << std::endl;
            return nullptr;
        }

        transform->SetWorldPosition(SnapToGrid(position));
        transform->SetWorldScale(width / gridSize, height / gridSize);

        auto renderer = wallObject->AddComponent<SpriteRendererComponent>();
        if (!renderer) return nullptr;

        renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("wall"));
        renderer->SetPixelSize(gridSize, gridSize);

        auto wallComponent = wallObject->AddComponent<WallComponent>();
        if (!wallComponent) return nullptr;

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
        if (!renderer) return nullptr;

        renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("floor"));
        renderer->SetPixelSize(gridSize, gridSize);

        auto floorComponent = floorObject->AddComponent<FloorComponent>();
        if (!floorComponent) return nullptr;

        return floorObject;
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
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        file << data.objects.size() << "\n";

        for (const auto& obj : data.objects)
        {
            file << obj.type << "\n";
            file << obj.position.x << " " << obj.position.y << "\n";
            file << obj.size.x << " " << obj.size.y << "\n";
            file << obj.textureName << "\n";
        }
    }

    LevelEditor::LevelData LevelEditor::LoadFromFile(const std::string& filename)
    {
        LevelData data;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cout << "Failed to open level file: " << filename << std::endl;
            return data;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line == "---")
            {
                LevelData::ObjectData objectData;

                std::getline(file, line);
                objectData.type = line.substr(6);

                std::getline(file, line);
                std::istringstream posStream(line.substr(10));
                posStream >> objectData.position.x >> objectData.position.y;

                std::getline(file, line);
                std::istringstream sizeStream(line.substr(6)); 
                sizeStream >> objectData.size.x >> objectData.size.y;

                std::getline(file, line);
                objectData.textureName = line.substr(9);

                data.objects.push_back(objectData);
            }
        }

        return data;
    }
}