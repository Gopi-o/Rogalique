// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

// #include "Application.h"
#include <SFML/Graphics.hpp>
#include "Core/Characters/Player.h"
#include <Systems/Logger.h>
#include <Core/GameEngine/GameEngine.h>
#include <Systems/Resource/ResourceSystem.h>
#include "Core/Levels/DeveloperLevel.h"
#include <iostream>
#include <windows.h>

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

	Engine::ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");
	Engine::ResourceSystem::Instance()->LoadTexture("enemy", "Resources/Textures/enemy.png");

	auto developerLevel = std::make_shared<DeveloperLevel>();
	developerLevel->Start();

	Engine::GameEngine::Instance()->SetCurrentScene(developerLevel.get());

	Engine::GameEngine::Instance()->Run();

	return 0;
}
