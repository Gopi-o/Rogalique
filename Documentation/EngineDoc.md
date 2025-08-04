# Документация движка (Engine)

## Основные системы

### GameWorld (Мир игры)
**Расположение**: `Core/GameWorld/GameWorld.h`, `Core/GameWorld/GameWorld.cpp`

#### Основные функции:
- `Instance()` - синглтон для доступа к миру
- `Update(deltaTime)` - обновление всех игровых объектов
- `Render()` - отрисовка всех игровых объектов
- `CreateGameObject()` - создание нового объекта
- `DestroyGameObject()` - пометка объекта на удаление
- `Find` методы для поиска объектов:
  - `FindObjectByName()`
  - `FindObjectByTag()`
  - `FindClosestObject()`
  - `FindObjectsInRadius()`

#### Особенности:
- Двухэтапное удаление объектов (пометка + фактическое удаление в LateUpdate)
- Поддержка иерархии объектов через TransformComponent

### GameObject (Игровой объект)
**Расположение**: `Core/GameObject/GameObject.h`, `Core/GameObject/GameObject.cpp`

#### Компонентная система:
```cpp
// Добавление компонента
auto* transform = AddComponent<TransformComponent>();

// Получение компонента
auto* renderer = GetComponent<SpriteRendererComponent>();
```
## Основные методы:
- `Update()`/`Render()` - вызов соответствующих методов компонентов

### Методы для работы с компонентами:
- `AddComponent<T>()`
- `GetComponent<T>()`
- `GetComponents<T>()`
- `RemoveComponent()`
- Поддержка тегов и имен

---

### GameEngine (Ядро движка)
**Расположение**: `Core/GameEngine/GameEngine.h`, `Core/GameEngine/GameEngine.cpp`

#### Игровой цикл:
1. Обработка событий (Input)
2. Обновление состояния (Update)
3. Физика (Physics)
4. Отрисовка (Render)
5. Удаление объектов (LateUpdate)

```cpp
void GameEngine::Run() {
    while (window.isOpen()) {
        // 1. Input
        while (window.pollEvent(event)) {
            GameStateManager::HandleEvent(event);
        }
        
        // 2. Update
        GameStateManager::UpdateCurrentScene(deltaTime);
        GameWorld::Instance()->Update(deltaTime);
        
        // 3. Physics
        PhysicsSystem::Instance()->Update();
        
        // 4. Render
        GameWorld::Instance()->Render();
        
        // 5. Cleanup
        GameWorld::Instance()->LateUpdate();
    }
}
```
## GameStateManager (Менеджер состояний)
**Расположение**: `Core/GameStateManager.h`, `Core/GameStateManager.cpp`

### Функционал:
- Регистрация сцен
- Переключение между сценами
- Управление жизненным циклом сцен:
  - `Start()`
  - `Stop()`
  - `Restart()`
  - `Update()`

```cpp
// Регистрация сцены
GameStateManager::Instance()->RegisterScene("MainMenu", 
    std::make_unique<MainMenuScene>());

// Переключение сцены
GameStateManager::Instance()->SwitchToScene("GameLevel");
```
## PhysicsSystem (Физическая система)
**Расположение**: `Systems/Physics/PhysicsSystem.h`, `Systems/Physics/PhysicsSystem.cpp`

### Особенности:
- Обнаружение коллизий
- Обработка триггеров
- Разрешение столкновений:
  - Учет кинематических объектов
  - Специальная обработка "пола"
  - Разделение по минимальной оси пересечения

```cpp
// Подписка коллайдера
PhysicsSystem::Instance()->Subscribe(collider);

// Обработка в компоненте:
void OnCollision(Collision& collision) override;
void OnTriggerEnter(Trigger& trigger) override;
```
## ResourceSystem (Система ресурсов)
**Расположение**: `Systems/Resource/ResourceSystem.h`, `Systems/Resource/ResourceSystem.cpp`

### Поддерживаемые ресурсы:
- Текстуры (`sf::Texture`)
- Шрифты (`sf::Font`)
- Текстуры-атласы (текстурные карты)

### Основные методы:
```cpp
// Загрузка
LoadTexture("player", "Assets/player.png");
LoadFont("main_font", "Fonts/Roboto.ttf");

// Получение
auto* texture = ResourceSystem::GetTextureShared("player");
auto* font = ResourceSystem::GetFontShared("main_font");
```
