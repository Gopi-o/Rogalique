#include "pch.h"
#include "InputComponent.h"


namespace Engine
{
	InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject) {}

	void InputComponent::Update(float deltaTime)
	{
		VerticalAxis = 0.f;
		horizontalAxis = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			VerticalAxis += 1.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			VerticalAxis -= 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			horizontalAxis -= 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			horizontalAxis += 1.f;
		}
	}

	void InputComponent::Render()
	{
	}
	float InputComponent::GetHorizontalAxis() const 
	{
		return horizontalAxis;
	}
	float InputComponent::GetVerticalAxis() const
	{
		return VerticalAxis;
	}
}
