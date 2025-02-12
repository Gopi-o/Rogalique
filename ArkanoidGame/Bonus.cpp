#include "Bonus.h"
#include "GameSettings.h"
#include "Sprite.h"

#include <assert.h>


namespace
{
	const std::string TEXTURE_ID = "platform"; //bonus_ball
}

namespace ArkanoidGame
{
	Bonus::Bonus(const sf::Vector2f& position, const sf::Color& color, BonusType type)
		: GameObject(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.BONUS_SIZE, SETTINGS.BONUS_SIZE)
		, type(type)
	{
		sprite.setColor(color);
	}


	void Bonus::Update(float timeDelta)
	{
		if (active)
		{
			sprite.move(0.f, fallSpeed * timeDelta); // Падение вниз
			// Проверка на выход за нижнюю границу
			if (GetPosition().y + SETTINGS.PLATFORM_HEIGHT > SETTINGS.SCREEN_HEIGHT)
			{
				active = false;
			}
		}
	}

	void Bonus::OnHit()
	{
		active = false; // Сделать неактивным при сборе
	}
	bool Bonus::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
	{
		auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
		assert(gameObject);
		sf::Rect rect = gameObject->GetRect();
		rect.width *= 1.1;
		return GetRect().intersects(gameObject->GetRect());
	}
}
