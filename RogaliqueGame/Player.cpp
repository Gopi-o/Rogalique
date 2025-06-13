#include "Player.h"
#include "SoundComponent.h"
#include <ResourceSystem.h>
#include <filesystem>
#include <LevelEditor.h>


namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject();
		gameObject->SetTag("Player");

		if (!gameObject) {
			std::cout << "Player::Update: gameObject is nullptr!" << std::endl;
			return;
		}
		
		// Добавляем компоненты
		auto transform = gameObject->AddComponent<Engine::TransformComponent>();
		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		auto soundComponent = gameObject->AddComponent<Engine::SoundComponent>();
		auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
		auto playerInput = gameObject->AddComponent<Engine::InputComponent>();
		auto collidery = gameObject->AddComponent<Engine::SpriteColliderComponent>();


		
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
		soundComponent->SetVolume(10.0f);
		soundComponent->SetLoop(true);
		soundComponent->Play();



		
		playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		playerCamera->SetBaseResolution(1280, 720);


		moveSpeed = 0.6f;
	}

	void Player::Update(float deltaTime)
	{
		auto input = gameObject->GetComponent<Engine::InputComponent>();
		auto rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();

		if (input && rigidbody)
		{
			float horizontal = input->GetHorizontalAxis();
			float vertical = input->GetVerticalAxis();

			Engine::Vector2Df movement(horizontal * moveSpeed, vertical * moveSpeed);

			rigidbody->SetLinearVelocity(movement);
		}
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}