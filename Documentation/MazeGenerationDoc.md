# Генератор лабиринта

## Компонент MazeGeneratorComponent
**Расположение**:  
`Components/Render/MazeGeneratorComponent.h`  
`Components/Render/MazeGeneratorComponent.cpp`

### Основные функции:
```cpp
// Главная функция генерации
void Generate(int width, int height, float cellSize, float wallThickness);

// Получение стартовой позиции
Vector2Df GetStartPointPos() const;

// Генерация точек спавна
void GenerateSpawnPoints(int enemyCount, int itemCount);
```


## Параметры генерации

| Параметр      | Тип    | Описание                          | Значение по умолчанию |
|---------------|--------|-----------------------------------|-----------------------|
| `width`       | `int`  | Ширина лабиринта в клетках       | `10`                  |
| `height`      | `int`  | Высота лабиринта в клетках       | `10`                  |
| `cellSize`    | `float`| Размер клетки в пикселях         | `812.0f`              |
| `wallThickness`| `float`| Толщина стен в пикселях          | `48.0f`               |

## Алгоритм работы

### 1. Инициализация сетки (`InitializeGrid()`)
- Создание двумерного массива клеток
- Выбор случайной стартовой позиции

### 2. Генерация лабиринта (`GenerateMazeDFS()`)
- Алгоритм поиска в глубину (DFS)
- Рекурсивное удаление стен между клетками
- Сохранение глубины каждой клетки

### 3. Создание стен (`CreateMazeWalls()`)
- Генерация GameObject'ов для каждой стены
- Использование `WallComponent`

### 4. Расстановка точек
- Стартовая точка `(0, 0)`
- Конечная точка (самая дальняя по глубине)

```cpp
// Пример использования
auto* generator = gameObject->AddComponent<MazeGeneratorComponent>();
generator->Generate(15, 15, 100.0f, 10.0f);
```


## Вспомогательные компоненты

### WallComponent (Компонент стены)
**Расположение**:  
`Components/Render/Wall/WallComponent.h`  
`Components/Render/Wall/WallComponent.cpp`

#### Особенности:
- Наследуется от `StaticColliderComponent`
- Создает неподвижные препятствия
- Автоматически обрабатывает коллизии
- Используется для построения стен лабиринта

```cpp
// Пример создания стены
auto* wall = LevelEditor::Instance()->CreateWall(position, width, height);
```


## FloorComponent (Компонент пола)
**Расположение**:  
`Components/Render/Floor/FloorComponent.h`  
`Components/Render/Floor/FloorComponent.cpp`

### Особенности:
- Создает проходимые поверхности
- Может быть расширен для разных типов полов
- Не имеет коллизий (по умолчанию)

```cpp
// Пример создания пола
auto* floor = LevelEditor::Instance()->CreateFloor(position, width, height);
```


## LevelEditor (Система редактирования)
**Расположение**:  
`Editor/LevelEditor.h`  
`Editor/LevelEditor.cpp`

### Основные функции:
```cpp
// Создание объектов
GameObject* CreateWall(const Vector2Df& position, float width, float height);
GameObject* CreateFloor(const Vector2Df& position, float width, float height);
GameObject* CreatePointMarker(const Vector2Df& position, const std::string& type);

// Работа с уровнями
void SaveLevel(const std::string& LevelName);
void LoadLevel(const std::string& LevelName);
```

## Пример структуры лабиринта
```cpp
struct Cell {
    bool northWall = true;  // Стена сверху
    bool southWall = true;  // Стена снизу
    bool eastWall = true;   // Стена справа
    bool westWall = true;   // Стена слева
    int depth = -1;         // Глубина при генерации
};
```


