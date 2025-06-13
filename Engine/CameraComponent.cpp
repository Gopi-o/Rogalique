#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"

namespace Engine
{
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
	{
		view = new sf::View(sf::FloatRect(0, 0, 800, -600));
		transform = gameObject->GetComponent<TransformComponent>();
		
		if (!transform)
		{
			transform = gameObject->AddComponent<TransformComponent>();
		}
	}

	CameraComponent::~CameraComponent()
	{
		delete view;
	}

	void CameraComponent::Update(float deltaTime)
	{
		if (!transform || !view)
		{
			return;
		}

		auto position = transform->GetWorldPosition();
		auto rotation = transform->GetWorldRotate();

		view->setCenter(Convert<sf::Vector2f, Vector2Df>(position));
		view->setRotation(rotation);

		// Получаем окно напрямую из RenderSystem
		RenderSystem::Instance()->GetMainWindow().setView(*view);
	}

	void CameraComponent::Render()
	{
		// Пустой метод, так как отрисовка происходит в Update
	}

	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0)
		{
			std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
			return;
		}
		if (view)
		{
			view->zoom(newZoom);
		}
	}

	void CameraComponent::SetBaseResolution(int width, int height)
	{
		if (view)
		{
			view->reset(sf::FloatRect(0, 0, width, -height));
		}
	}
}
