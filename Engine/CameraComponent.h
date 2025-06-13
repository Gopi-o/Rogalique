#pragma once
#include "Component.h"
#include "SFML/Graphics.hpp"

namespace Engine
{
	class TransformComponent;

	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject);
		~CameraComponent();

		void Update(float deltaTime) override;
		void Render() override;
		void ZoomBy(float newZoom);
		void SetBaseResolution(int width, int height);

	private:
		sf::View* view;
		TransformComponent* transform;
	};
}

