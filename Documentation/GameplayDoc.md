# Gameplay Documentation

## 1. Stats System
**Локация:** `Components/GamePlay/Stats/StatsComponent`

### Описание
Универсальная система хранения и управления статистиками объектов (HP, урон, скорость и т.д.).

### Ключевые особенности
- Динамическое добавление/изменение значений любого типа через `std::any`
- Поддержка колбэков при изменении значений
- Шаблонные методы для безопасного доступа к данным

### API
```cpp
// Установка значения
template <typename T>
void SetValue(const std::string& key, const T& value);

// Получение значения
template <typename T>
T GetValue(const std::string& key) const;

// Проверка наличия значения
template <typename T>
bool HasValue(const std::string& key) const;

// Колбэк при изменении
void SetOnValueChangedCallback(ValueChangedCallback callback);
```
### Пример использования
```cpp
stats->SetValue<float>("Health", 100.0f);
float hp = stats->GetValue<float>("Health");
```

## **2. Input System**
**Локация:** `Components/GamePlay/Input/InputComponent`

### **Описание**
Обработка ввода игрока с клавиатуры.

### **Поддерживаемые действия**
- Движение (WASD/Стрелки)
- Атака (Пробел)
- Пауза (Escape)

### **Методы**
```cpp
float GetHorizontalAxis() const; // [-1..1]
float GetVerticalAxis() const;   // [-1..1]
bool IsAttack() const;          // Однократное нажатие
bool IsPause() const;           // Триггер паузы
```

## **3. Effect System**
**Локация:** `Components/GamePlay/Effect/EffectComponent`

### **Описание**
Система визуальных эффектов с временными модификациями.

### **Типы эффектов**
- **Transform Effects**: Изменение позиции/масштаба
- **Color Effects**: Изменение цвета
- **Composite Effects**: Комбинации эффектов

### **Пример эффекта удара**
```cpp
AddHitEffect(0.2f); // Длительность 0.2 сек
```

## **4. Damage System**
**Локация:** `Components/GamePlay/Damageable/DamageableComponent`

### **Механика**
- Обработка входящего урона
- Модификаторы урона
- Режим неуязвимости

### **Ключевые методы**
```cpp
void TakeDamage(float damage, GameObject* source = nullptr);
void SetInvincible(bool invincible);
void SetDamageMultiplier(float multi);
```

## **5. Attack System**
**Локация:** `Components/GamePlay/AttackSystems/AttackSystem`

### **Типы атак**

| Тип          | Описание               |
|--------------|------------------------|
| Melee        | Ближний бой            |
| Ranged       | Дальняя атака          |
| AreaOfEffect | Область эффекта        |
| Projectile   | Снаряды                |

### **Параметры атаки**
```cpp
struct AttackParams {
    AttackType type;
    float damage;
    float range;
    float cooldown;
    float areaRadius;
    std::string animationName;
    std::string soundName;
};
```

### Логика работы
- Проверка кд-таймеров
- Поиск целей в радиусе (для Melee/AoE)
- Нанесение урона через DamageableComponent
- Визуальные/звуковые эффекты
- Активация кд

### Пример добавления атаки
```cpp
AttackSystem* attack = obj->AddComponent<AttackSystem>();
attack->AddAttack("Slash", {
    .type = AttackType::Melee,
    .damage = 15.0f,
    .range = 50.0f,
    .cooldown = 1.0f
});
```
