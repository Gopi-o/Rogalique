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

		// ��������� ��������� � �����������
		void Generate(int width, int height, float cellSize = 512.0f, float wallThickness = 64.0f);
		void Clear();

		void Update(float deltaTime) override {}
		void Render() override {}

		// ��������������� ������
		std::vector<Vector2Df> GetEnemySpawnPoints() const { return enemySpawnPoints; }
		std::vector<Vector2Df> GetItemSpawnPoints() const { return itemSpawnPoints; }
		Vector2Df GetStartPointPos() const;

	private:
		struct Cell
		{
			bool northWall = true;
			bool southWall = true;
			bool eastWall = true;
			bool westWall = true;
			int depth = -1;
		};

		// ��������� ���������
		int mazeWidth = 10;
		int mazeHeight = 10;
		float cellSize = 64.0f;
		float wallThickness = 8.0f;

		// ��������������� ������
		GameObject* StartPoint = nullptr;
		GameObject* EndPoint = nullptr;
		void GenerateSpawnPoints(int enemyCount, int itemCount);
		std::vector<Vector2Df> enemySpawnPoints;
		std::vector<Vector2Df> itemSpawnPoints;

		// ������ ���������
		std::vector<std::vector<Cell>> mazeGrid;
		std::stack<Vector2Df> generationStack;

		// ������ ���������
		void InitializeGrid();
		void GenerateMazeDFS();
		void CreateMazeWalls();
		void CreateMazeFloor();
		void CreateWall(const Vector2Df& position, float width, float height);
		void CreateFloor(const Vector2Df& position, float width, float height);

		// ��������������� ������
		std::vector<Vector2Df> GetUnvisitedNeighbors(int x, int y, const std::vector<std::vector<bool>>& visited);
		void SetStartAndEndPoints();
		Vector2Df FindFurthestPoint();
		void SpawnEnemies(int enemyCount);
	};
} // namespace Engine