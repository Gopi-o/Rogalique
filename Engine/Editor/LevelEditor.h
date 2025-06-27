#pragma once
#include "Core/GameObject/GameObject.h"
#include "Math/Vector.h"
#include "Components/Render/Wall/WallComponent.h"
#include "Components/Render/Floor/FloorComponent.h"
#include "Components/Render/SpriteRenderer/SpriteRendererComponent.h"
#include "Components/Physics/Transform/TransformComponent.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace Engine
{
	class LevelEditor
	{
	public:
		static LevelEditor* Instance();

		void Update(float deltaTime);
		void Render();

		GameObject* CreateWall(const Vector2Df& position, float width, float height);
		GameObject* CreateFloor(const Vector2Df& position, float width, float height);
		GameObject* CreatePointMarker(const Vector2Df& position, const std::string& type);

		void SaveLevel(const std::string& LevelName);
		void LoadLevel(const std::string& LevelName);

		void SetGridSize(float size);
		void SetGridVisible(bool isVisible);
		Vector2Df SnapToGrid(const Vector2Df& position) const;

	private:
		LevelEditor() {}
		~LevelEditor() {}

		LevelEditor(LevelEditor const&) = delete;
		LevelEditor& operator=(LevelEditor const&) = delete;

		float gridSize = 32.0f;
		bool isGridVisible = true;

		struct LevelData
		{
			struct ObjectData
			{
				std::string type;
				Vector2Df position;
				Vector2Df size;
				std::string textureName;
			};
			std::vector<ObjectData> objects;
		};

		void DrawGrid();
		void SaveToFile(const std::string& filename, const LevelData& data);
		LevelData LoadFromFile(const std::string& filename);
	};
} // namespace Engine
