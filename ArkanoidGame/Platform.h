#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Collidable.h"

namespace ArkanoidGame
{
	class Platform : public GameObject, public Colladiable
	{
	public:
		Platform(const sf::Vector2f& position);
		void Update(float timeDelta) override;
		
		bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;
		void OnHit() override {}
		bool CheckCollision(std::shared_ptr<Colladiable> collidable) override;

		void SetSize(float width);
		void SetSpeed(float speed);
		void SetBounceDirection(float angle);

		float GetSpeed() const;
		float GetWidth() const;

	private:
		void Move(float speed);

		float platformSpeed;
		float Vwidth;
	};
}