// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

// #include "Application.h"
#include <SFML/Graphics.hpp>
#include "Core/Characters/Player.h"
#include <Systems/Logger.h>
#include <Core/GameEngine/GameEngine.h>
#include <Systems/Resource/ResourceSystem.h>
#include "Core/Levels/DeveloperLevel.h"
#include <Core/GameStateManager.h>
#include <iostream>
#include <windows.h>
#include "Scenes/MainMenuScene.h"

using namespace RogaliqueGame;

int main()
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);

	setupLogger();

	LOG_INFO("Started");

	Engine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "My Game Roguelike"));

	ResourceSystem::Instance()->LoadTexture("wall", "Resources/Textures/wall1.jpg");
	ResourceSystem::Instance()->LoadTexture("floor", "Resources/Textures/floor.png");
	ResourceSystem::Instance()->LoadTexture("start", "Resources/Textures/start.png");
	ResourceSystem::Instance()->LoadTexture("exit", "Resources/Textures/exit.png");
	ResourceSystem::Instance()->LoadTexture("health_icon", "Resources/Textures/BodyBend.png");
	Engine::ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/player000.png");
	Engine::ResourceSystem::Instance()->LoadTexture("enemy", "Resources/Textures/enemy0.png");
	ResourceSystem::Instance()->LoadTexture("button_normal_menu", "Resources/Textures/nomal_menus_Button.png");
	ResourceSystem::Instance()->LoadTexture("button_hovered", "Resources/Textures/button_hovere.png");
	ResourceSystem::Instance()->LoadTexture("button_pressed", "Resources/Textures/button_press.png");
	ResourceSystem::Instance()->LoadTexture("menu_background", "Resources/Textures/main_Background.png");
	Engine::ResourceSystem::Instance()->LoadFont("default_font", "Resources/Fonts/Roboto-Regular.ttf");
	ResourceSystem::Instance()->LoadFont("menu_font", "Resources/Fonts/Roboto-Light.ttf");

	// Где-то при инициализации игры
	auto& stateManager = *Engine::GameStateManager::Instance();
	stateManager.RegisterScene("MainMenu", std::make_unique<MainMenuScene>());
	stateManager.RegisterScene("Level1", std::make_unique<DeveloperLevel>());

	// Переключение на главное меню
	stateManager.SwitchToScene("MainMenu");

	// auto developerLevel = std::make_shared<DeveloperLevel>();
	// developerLevel->Start();

	// Engine::GameEngine::Instance()->SetCurrentScene(developerLevel.get());

	Engine::GameEngine::Instance()->Run();

	return 0;
}
