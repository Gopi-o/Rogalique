#include "Player.h"
#include "SoundComponent.h"
#include <ResourceSystem.h>
#include <filesystem>


namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject();
		gameObject->SetTag("Player");

		
		// Добавляем компоненты
		auto transform = gameObject->AddComponent<Engine::TransformComponent>();
		auto soundComponent = gameObject->AddComponent<Engine::SoundComponent>();
		
		std::string soundPath = "Resources\\Sounds\\Death.wav";
		/*if (!std::filesystem::exists(soundPath))
		{
			std::cout << "Sound file does not exist at path: " << soundPath << std::endl;
			std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
			return;
		}*/

		if (!soundComponent->LoadFromFile(soundPath))
		{
			std::cout << "Failed to load sound file!" << std::endl;
			return;
		}
		soundComponent->SetVolume(0.0f);
		soundComponent->SetLoop(true);
		soundComponent->Play();
		
		auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
		playerCamera->SetBaseResolution(1280, 720);

		auto playerInput = gameObject->AddComponent<Engine::InputComponent>();
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}