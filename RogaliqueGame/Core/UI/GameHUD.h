#pragma once
#include <Components/UI/Canvas.h>
#include <Components/UI/TextBlock.h>
#include <Components/UI/Image.h>
#include <Components/UI/Layout.h>

namespace RogaliqueGame
{
	class GameHUD
	{
	public:
		GameHUD();
		~GameHUD() = default;

		void Initialize();
		void Update(float deltaTime);
		void Render();
		void HandleEvent(const sf::Event& event);

		void SetHealth(int current, int max);
		void SetAmmo(int current, int max);
		void SetEnemiesCount(int count);
		void SetLevelInfo(const std::string& info);

	private:
		Engine::Canvas* canvas = nullptr;
		Engine::VerticalBox* mainContainer = nullptr;
		Engine::HorizontalBox* topPanel = nullptr;
		Engine::HorizontalBox* bottomPanel = nullptr;

		Engine::TextBlock* healthText = nullptr;
		Engine::Image* healthIcon = nullptr;
		Engine::TextBlock* ammoText = nullptr;
		Engine::TextBlock* enemiesText = nullptr;
		Engine::TextBlock* levelText = nullptr;
	};
} // namespace RogaliqueGame
