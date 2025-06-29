#pragma once
#include "Components/Component.h"
#include <Components/Physics/Actor/A_test/LevelPointsComponent.h>
#include "Math/Vector.h"
#include <vector>
#include <stack>

namespace Engine
{
	class MazeGeneratorComponent : public Component
	{
	public:
		MazeGeneratorComponent(GameObject* gameObject);

		// Генерация лабиринта с параметрами
		void Generate(int width, int height, float cellSize = 256.0f, float wallThickness = 24.0f);
		void Clear();

		void Update(float deltaTime) override {}
		void Render() override {}

	private:
		struct Cell
		{
			bool northWall = true;
			bool southWall = true;
			bool eastWall = true;
			bool westWall = true;
			int depth = -1;
		};

		// Параметры лабиринта
		int mazeWidth = 10;
		int mazeHeight = 10;
		float cellSize = 64.0f;
		float wallThickness = 8.0f;

		// Вспомогательные данные
		GameObject* StartPoint = nullptr;
		GameObject* EndPoint = nullptr;

		// Данные лабиринта
		std::vector<std::vector<Cell>> mazeGrid;
		std::stack<Vector2Df> generationStack;

		// Методы генерации
		void InitializeGrid();
		void GenerateMazeDFS();
		void CreateMazeWalls();
		void CreateWall(const Vector2Df& position, float width, float height);

		// Вспомогательные методы
		std::vector<Vector2Df> GetUnvisitedNeighbors(int x, int y, const std::vector<std::vector<bool>>& visited);
		void SetStartAndEndPoints();
		Vector2Df FindFurthestPoint();
		void SpawnEnemies(int enemyCount);
	};
} // namespace Engine