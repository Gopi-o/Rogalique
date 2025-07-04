#include "GameHUD.h"
#include <Systems/Resource/ResourceSystem.h>
#include <Systems/Render/RenderSystem.h>
#include <Core/GameWorld/GameWorld.h>

namespace RogaliqueGame
{
	GameHUD::GameHUD()
	{
		Initialize();
		this->canvas = canvas;
	}

	void GameHUD::Initialize()
	{
		auto hudObject = Engine::GameWorld::Instance()->CreateGameObject();
		hudObject->SetTag("HUD");
		this->canvas = (hudObject->AddComponent<Engine::Canvas>());
		canvas->SetScreenSpace(true);

		Engine::ResourceSystem::Instance()->LoadFont("default_font", "Resources/Fonts/Roboto-Regular.ttf");

		auto iconObject = Engine::GameWorld::Instance()->CreateGameObject();
		iconObject->SetTag("health_icon");
		this->healthIcon = (iconObject->AddComponent<Engine::Image>());
		healthIcon->SetTexture("health_icon");
		healthIcon->SetSize({ 50.f, 50.f });
		healthIcon->SetOrigin(0, 0);
		healthIcon->SetColor(sf::Color(255, 255, 255, 200));

		auto containerObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->mainContainer = (containerObject->AddComponent<Engine::VerticalBox>());
		mainContainer->SetAutoSize(true);
		mainContainer->SetSpacing(20.f);
		mainContainer->SetPadding({ 20.f, 20.f });

		auto topPanelObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->topPanel = (topPanelObject->AddComponent<Engine::HorizontalBox>());
		topPanel->SetAutoSize(true);
		topPanel->SetSpacing(70.f);
		topPanel->SetPadding({ 10.f, 10.f });

		auto healthTextObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->healthText = (healthTextObject->AddComponent<Engine::TextBlock>());
		healthText->SetSize({ 200.f, 30.f });
		healthText->SetFont("default_font");
		healthText->SetFontSize(24);
		healthText->SetTextColor(sf::Color::White);
		healthText->SetOutlineColor(sf::Color::Black);
		healthText->SetOutlineThickness(1.f);
		healthText->SetText("Health: 100/100");

		auto ammoTextObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->ammoText = (ammoTextObject->AddComponent<Engine::TextBlock>());
		ammoText->SetFont("default_font");
		ammoText->SetFontSize(24);
		ammoText->SetTextColor(sf::Color::White);
		ammoText->SetOutlineColor(sf::Color::Black);
		ammoText->SetOutlineThickness(1.f);
		ammoText->SetText("Ammo: 30/120");

		topPanel->AddChild(healthText);
		topPanel->AddChild(ammoText);

		auto bottomPanelObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->bottomPanel = (bottomPanelObject->AddComponent<Engine::HorizontalBox>());
		bottomPanel->SetAutoSize(true);
		bottomPanel->SetSpacing(10.f);
		bottomPanel->SetPadding({ 10.f, 40.f });

		auto enemiesTextObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->enemiesText = (enemiesTextObject->AddComponent<Engine::TextBlock>());
		enemiesText->SetFont("default_font");
		enemiesText->SetFontSize(24);
		enemiesText->SetTextColor(sf::Color::White);
		enemiesText->SetOutlineColor(sf::Color::Black);
		enemiesText->SetOutlineThickness(1.f);
		enemiesText->SetText("Enemies: 5");

		auto levelTextObject = Engine::GameWorld::Instance()->CreateGameObject();
		this->levelText = (levelTextObject->AddComponent<Engine::TextBlock>());
		levelText->SetFont("default_font");
		levelText->SetFontSize(24);
		levelText->SetTextColor(sf::Color::White);
		levelText->SetOutlineColor(sf::Color::Black);
		levelText->SetOutlineThickness(1.f);
		levelText->SetText("Level: 1");

		bottomPanel->AddChild(enemiesText);
		bottomPanel->AddChild(levelText);

		mainContainer->AddChild(topPanel);
		mainContainer->AddChild(bottomPanel);
		mainContainer->AddChild(healthIcon);

		// hudObject->SetShouldRender(false);
		iconObject->SetShouldRender(false);
		containerObject->SetShouldRender(false);
		topPanelObject->SetShouldRender(false);
		healthTextObject->SetShouldRender(false);
		ammoTextObject->SetShouldRender(false);
		bottomPanelObject->SetShouldRender(false);
		enemiesTextObject->SetShouldRender(false);
		levelTextObject->SetShouldRender(false);

		canvas->AddToLayer(mainContainer, 100);
	}

	void GameHUD::Update(float deltaTime)
	{
		if (canvas)
		{
			canvas->Update(deltaTime);
		}
	}

	void GameHUD::Render()
	{
		if (canvas)
		{
			canvas->Render();
		}
	}

	void GameHUD::HandleEvent(const sf::Event& event)
	{
		if (canvas)
		{
			canvas->HandleEvent(event);
		}
	}

	void GameHUD::SetHealth(int current, int max)
	{
		if (healthText)
		{
			healthText->SetText("Health: " + std::to_string(current) + "/" + std::to_string(max));
		}
	}

	void GameHUD::SetAmmo(int current, int max)
	{
		if (ammoText)
		{
			ammoText->SetText("Ammo: " + std::to_string(current) + "/" + std::to_string(max));
		}
	}

	void GameHUD::SetEnemiesCount(int count)
	{
		if (enemiesText)
		{
			enemiesText->SetText("Enemies: " + std::to_string(count));
		}
	}

	void GameHUD::SetLevelInfo(const std::string& info)
	{
		if (levelText)
		{
			levelText->SetText("Level: " + info);
		}
	}

} // namespace RogaliqueGame