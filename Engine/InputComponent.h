#pragma once
#include "Component.h"
#include <SFML/Window.hpp>

namespace Engine
{
	class InputComponent : public Component
	{
		public: 
			InputComponent(GameObject* gameObject);

			void Update(float deltaTime) override;
			void Render() override;

			float GetHorizontalAxis() const;
			float GetVerticalAxis() const;
			bool IsAttack() const;

	private:
		float horizontalAxis = 0.f;
		float VerticalAxis = 0.f;
		bool isAttack = false;
	};
}

