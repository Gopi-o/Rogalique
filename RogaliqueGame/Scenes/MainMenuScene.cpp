#include "MainMenuScene.h"
#include <Systems/Resource/ResourceSystem.h>
#include <filesystem>
#include <Systems/Logger.h>
#include <Components/Render/MazeGeneratorComponent.h>
#include "../Stats/UnitStatsComponent.h"
#include <Core/GameWorld/GameWorld.h>
#include <Components/UI/Layout.h>
#include <Components/UI/TextBlock.h>
#include <Core/GameStateManager.h>

using namespace Engine;

namespace RogaliqueGame
{
	void MainMenuScene::Start()
	{
		LOG_INFO("MainMenuScene is started;");

		 // Create canvas for the menu
		auto menuObject = GameWorld::Instance()->CreateGameObject();
		menuObject->SetTag("MainMenu");
		canvas = menuObject->AddComponent<Canvas>();
		canvas->SetScreenSpace(true);

		// Load resources


		// Create background
		auto bgObject = GameWorld::Instance()->CreateGameObject();
		bgObject->SetTag("bgObject");
		background = bgObject->AddComponent<Image>();
		auto windowSize = RenderSystem::Instance()->GetMainWindow().getSize();
		background->SetTexture("menu_background");
		background->SetSize({ static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });

		// Create main container
		auto containerObject = GameWorld::Instance()->CreateGameObject();
		containerObject->SetTag("containerObject");
		mainContainer = containerObject->AddComponent<VerticalBox>();
		mainContainer->SetSize({ 100.f, 404.f });
		mainContainer->SetAutoSize(false);
		mainContainer->SetPosition({ static_cast<float>(windowSize.x) / 2 - 200.f, 150.f });
		mainContainer->SetSpacing(20.f);
		mainContainer->SetPadding({ 1.f, 20.f });

		// Title text
		auto titleObject = GameWorld::Instance()->CreateGameObject();
		titleText = titleObject->AddComponent<TextBlock>();
		titleText->SetFont("menu_font");
		titleText->SetFontSize(48);
		titleText->SetTextColor(sf::Color::White);
		titleText->SetOutlineColor(sf::Color::Black);
		titleText->SetOutlineThickness(2.f);
		titleText->SetText("ROGALIQUE");
		// If SetAlignment doesn't exist, you might need to center it manually
		// titleText->SetOrigin(titleText->GetLocalBounds().width / 2, 0);

		// Create buttons
		CreateButton("New Game", [this]() { OnNewGameClicked(); });
		CreateButton("Load Game", [this]() { OnLoadGameClicked(); });
		CreateButton("Options", [this]() { OnOptionsClicked(); });
		CreateButton("Exit", [this]() { OnExitClicked(); });

		// Add elements to containers
		mainContainer->AddChild(titleText);

		// hudObject->SetShouldRender(false);
		bgObject->SetShouldRender(false);
		containerObject->SetShouldRender(false);
		titleObject->SetShouldRender(false);
		

		// Add containers to canvas
		canvas->AddToLayer(background, 0);
		canvas->AddToLayer(mainContainer, 1);
	}

	void MainMenuScene::Restart()
	{
		LOG_INFO("MainMenuScene is Restart;");
		Stop();
		Start();
	}

	void MainMenuScene::Stop()
	{
		LOG_INFO("MainMenuScene is Stop;");
		// Clean up resources
		if (canvas && canvas->GetGameObject())
		{
			GameWorld::Instance()->DestroyGameobject(canvas->GetGameObject());
		}

		for (auto& button : menuButtons)
		{
			if (button && button->GetGameObject())
			{
				GameWorld::Instance()->DestroyGameobject(button->GetGameObject());
			}
		}
		menuButtons.clear();
	}

	void MainMenuScene::Update(float deltaTime)
	{
		//LOG_INFO("MainMenuScene is Update;");
		if (canvas)
		{
			canvas->Update(deltaTime);
		}
	}

	void MainMenuScene::HandleEvent(const sf::Event& event)
	{

		// Передаём события UI
		if (canvas)
		{
			canvas->HandleEvent(event);
		}
	}

	void MainMenuScene::CreateButton(const std::string& text, std::function<void()> onClick)
	{
		auto buttonObject = GameWorld::Instance()->CreateGameObject();
		auto button = buttonObject->AddComponent<Button>();
		buttonObject->SetTag(text);

		button->SetNormalTexture("button_normal_menu");
		button->SetHoveredTexture("button_hovered");
		button->SetPressedTexture("button_pressed");



		button->SetText(text);
		button->SetTextFont("menu_font");
		button->SetTextSize(24);
		button->SetTextColor(sf::Color::White);
		button->SetTextOutlineColor(sf::Color::Black);
		button->SetTextOutlineThicness(1.f); 

		button->SetSize({ 200.f, 60.f });
		button->SetInteractive(true);
		button->OnClick = onClick;

		if (button->GetSize().x <= 0 || button->GetSize().y <= 0)
		{
			LOG_DEBUG("Invalid button size: " + std::to_string(button->GetSize().x) + "x" + std::to_string(button->GetSize().y));
		}

		mainContainer->AddChild(button);
		menuButtons.push_back(button);
	
		buttonObject->SetShouldRender(false);
	}

	void MainMenuScene::OnNewGameClicked()
	{
		LOG_INFO("New Game button clicked");
		// Implement transition to game scene

		auto& stateManager = *Engine::GameStateManager::Instance();
		stateManager.SwitchToScene("Level1");
	}

	void MainMenuScene::OnLoadGameClicked()
	{
		LOG_INFO("Load Game button clicked");
		// Implement load game functionality
	}

	void MainMenuScene::OnOptionsClicked()
	{
		LOG_INFO("Options button clicked");
		// Implement options menu
	}

	void MainMenuScene::OnExitClicked()
	{
		LOG_INFO("Exit button clicked");
	}
}
