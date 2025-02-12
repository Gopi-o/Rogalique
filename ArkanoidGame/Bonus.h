#pragma once
#include "GameObject.h"
#include "Collidable.h"


enum class BonusType { 
	FireBall,
	FragileBlocks,
	IncreasePlatformSize,
	IncreasePlatformSpeed,
	BounceBallRandomDirection
};

namespace ArkanoidGame
{
	class Bonus : public GameObject, public Colladiable
	{
	public:
		Bonus(const sf::Vector2f& position, const sf::Color& color, BonusType type);
		virtual ~Bonus() = default;
		BonusType GetType() const { return type; }
		void Update(float timeDelta) override;
		void OnHit() override; // Для обработки сбора бонуса
		bool GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		bool IsActive() const { return active; }

	private:
		BonusType type;
		bool active = true;
		float fallSpeed = 200.f; // скорость падения бонуса
	};

	 
}
