#include "pch.h"
#include "MazeGeneratorComponent.h"
#include <Editor/LevelEditor.h>
#include <Systems/Logger.h>
#include <Math/RandomHelper.h>

namespace Engine
{
	MazeGeneratorComponent::MazeGeneratorComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	// Главная генерация лабиринта с параметрами
	void MazeGeneratorComponent::Generate(int width, int height, float cellSize, float wallThickness)
	{
		this->mazeWidth = std::max(5, width);
		this->mazeHeight = std::max(5, height);
		this->cellSize = cellSize;
		this->wallThickness = wallThickness;

		Clear();
		InitializeGrid();
		GenerateMazeDFS();
		CreateMazeWalls();
		SetStartAndEndPoints();
		GenerateSpawnPoints(5, 3);
	}

	// Можно расширить если надо будет что-то очищать.
	void MazeGeneratorComponent::Clear()
	{
		// Например очистка сгенерированных стен
	}

	// Получени позиции стартовой точки в лабиринте
	Vector2Df MazeGeneratorComponent::GetStartPointPos() const
	{
		Vector2Df pos;
		pos.x = StartPoint->GetComponent<TransformComponent>()->GetWorldPosition().x - 5.f;
		pos.y = StartPoint->GetComponent<TransformComponent>()->GetWorldPosition().y - 5.f;

		return pos;
	}

	// Генерация точек по кторым потом можно будет спавнить что-либо
	void MazeGeneratorComponent::GenerateSpawnPoints(int enemyCount, int itemCount)
	{
		enemySpawnPoints.clear();
		itemSpawnPoints.clear();

		// Переменная с позициями для спавна
		std::vector<Vector2Df> validPointsPosition;

		// Получение общее количество куда можно поставить спавнер
		for (int x = 0; x < mazeWidth; x++)
		{
			for (int y = 0; y < mazeHeight; y++)
			{
				Vector2Df pos(x * cellSize, y * cellSize);

				if (pos == StartPoint->GetComponent<TransformComponent>()->GetWorldPosition() || pos == EndPoint->GetComponent<TransformComponent>()->GetWorldPosition())
				{
					continue;
				}

				validPointsPosition.push_back(pos);
			}
		}

		// Заполнение точек для установления точек для спавнеров врагов
		for (int i = 0; i < enemyCount && !validPointsPosition.empty(); i++)
		{
			int index = RandomHelper::Int(0, validPointsPosition.size() - 1);
			enemySpawnPoints.push_back(validPointsPosition[index]);
			validPointsPosition.erase(validPointsPosition.begin() + index);
		}

		// Заполнение точек для установления точек для спавнеров итемов
		for (int i = 0; i < itemCount && !validPointsPosition.empty(); i++)
		{
			int index = RandomHelper::Int(0, validPointsPosition.size() - 1);
			itemSpawnPoints.push_back(validPointsPosition[index]);
			validPointsPosition.erase(validPointsPosition.begin() + index);
		}
	}

	// Инициализация сетки
	void MazeGeneratorComponent::InitializeGrid()
	{
		mazeGrid.resize(mazeHeight, std::vector<Cell>(mazeWidth));

		// Начальная позиция для генерации
		int startX = RandomHelper::Int(0, mazeWidth - 1);
		int startY = RandomHelper::Int(0, mazeHeight - 1);
		/*int startX = rand() % mazeWidth;
		int startY = rand() % mazeHeight;*/
		mazeGrid[startY][startX].depth = 0;

		generationStack.push(Vector2Df(startX, startY));
	}

	// Генерация лабиринта путем прохода в глубину
	void MazeGeneratorComponent::GenerateMazeDFS()
	{
		std::vector<std::vector<bool>> visited(mazeHeight, std::vector<bool>(mazeWidth, false));
		auto start = generationStack.top();
		mazeGrid[start.y][start.x].depth = 0; // Стартовая клетка имеет глубину 0
		visited[start.y][start.x] = true;

		while (!generationStack.empty())
		{
			auto current = generationStack.top();
			generationStack.pop();

			auto neighbors = GetUnvisitedNeighbors(current.x, current.y, visited);
			if (!neighbors.empty())
			{
				generationStack.push(current);

				// Выбор случайного соседа
				auto next = neighbors[RandomHelper::Int(0, neighbors.size() - 1)];

				// Устанавливаем глубину (текущая глубина + 1)
				mazeGrid[next.y][next.x].depth = mazeGrid[current.y][current.x].depth + 1;
				// auto next = neighbors[rand() % neighbors.size()];

				// Удаляем стену между текущей и следующей клеткой
				if (next.x == current.x + 1)
				{
					mazeGrid[current.y][current.x].eastWall = false;
					mazeGrid[next.y][next.x].westWall = false;
				}
				else if (next.x == current.x - 1)
				{
					mazeGrid[current.y][current.x].westWall = false;
					mazeGrid[next.y][next.x].eastWall = false;
				}
				else if (next.y == current.y + 1)
				{
					mazeGrid[current.y][current.x].southWall = false;
					mazeGrid[next.y][next.x].northWall = false;
				}
				else if (next.y == current.y - 1)
				{
					mazeGrid[current.y][current.x].northWall = false;
					mazeGrid[next.y][next.x].southWall = false;
				}

				visited[next.y][next.x] = true;
				generationStack.push(next);
			}
		}
	}

	std::vector<Vector2Df> MazeGeneratorComponent::GetUnvisitedNeighbors(int x, int y, const std::vector<std::vector<bool>>& visited)
	{
		std::vector<Vector2Df> neighbors;
		std::vector<Vector2Df> directions = { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

		for (const auto& dir : directions)
		{
			int nx = x + dir.x;
			int ny = y + dir.y;

			if (nx >= 0 && nx < mazeWidth && ny >= 0 && ny < mazeHeight && !visited[ny][nx])
			{
				neighbors.push_back(Vector2Df(nx, ny));
			}
		}

		return neighbors;
	}

	// Установление точек старта и конца
	void MazeGeneratorComponent::SetStartAndEndPoints()
	{
		if (StartPoint)
			LOG_WARN("Old Start points not deleted;");
		if (EndPoint)
			LOG_WARN("Old End points not deleted;");

		Vector2Df startPos(0, 0);
		StartPoint = LevelEditor::Instance()->CreatePointMarker(startPos, "StartPoint");
		StartPoint->GetComponent<TransformComponent>()->SetWorldPosition(startPos);
		// StartPoint->GetComponent<SpriteColliderComponent>();
		auto startComp = StartPoint->AddComponent<LevelPointsComponent>();
		startComp->SetStartPoint(StartPoint);

		LOG_INFO("Start point created at: (" + std::to_string(startPos.x) + ", " + std::to_string(startPos.y) + ")");

		Vector2Df exitPos = FindFurthestPoint();
		if (exitPos == startPos)
			exitPos = FindFurthestPoint();
		EndPoint = LevelEditor::Instance()->CreatePointMarker(exitPos, "ExitPoint");
		EndPoint->GetComponent<TransformComponent>()->SetWorldPosition(exitPos);
		EndPoint->GetComponent<SpriteColliderComponent>();
		auto exitComp = EndPoint->AddComponent<LevelPointsComponent>();
		exitComp->SetEndPoint(EndPoint);

		LOG_INFO("Exit point created at: (" + std::to_string(exitPos.x) + ", " + std::to_string(exitPos.y) + ")");
	}

	Vector2Df MazeGeneratorComponent::FindFurthestPoint()
	{
		Vector2Df furthestPoint(0, 0);
		int maxDepth = 0;

		for (int y = 0; y < mazeHeight; y++)
		{
			for (int x = 0; x < mazeWidth; x++)
			{
				if (mazeGrid[y][x].depth > maxDepth)
				{
					maxDepth = mazeGrid[y][x].depth;
					furthestPoint = Vector2Df(x * cellSize, y * cellSize);
				}
			}
		}

		return furthestPoint;
	}

	void MazeGeneratorComponent::CreateMazeWalls()
	{
		for (int y = 0; y < mazeHeight; y++)
		{
			for (int x = 0; x < mazeWidth; x++)
			{
				Vector2Df cellPos(x * cellSize, y * cellSize);

				if (mazeGrid[y][x].northWall)
					CreateWall(cellPos + Vector2Df(0, -cellSize / 2), cellSize + wallThickness, wallThickness);

				if (mazeGrid[y][x].southWall)
					CreateWall(cellPos + Vector2Df(0, cellSize / 2), cellSize + wallThickness, wallThickness);

				if (mazeGrid[y][x].westWall)
					CreateWall(cellPos + Vector2Df(-cellSize / 2, 0), wallThickness, cellSize + wallThickness);

				if (mazeGrid[y][x].eastWall)
					CreateWall(cellPos + Vector2Df(cellSize / 2, 0), wallThickness, cellSize + wallThickness);
			}
		}
	}

	void MazeGeneratorComponent::CreateWall(const Vector2Df& position, float width, float height)
	{
		auto wall = LevelEditor::Instance()->CreateWall(position, width, height);
	}
} // namespace Engine