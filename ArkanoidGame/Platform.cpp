#include "Platform.h"
#include "Ball.h"
#include "GameSettings.h"
#include "randomizer.h"
#include "BonusManager.h"
#include "Sprite.h"
#include <algorithm>
#include "PlatformSizeCommand.h"
#include "PlatformSpeedCommand.h"
#include "PlatformBounceCommand.h"

namespace
{
	const std::string TEXTURE_ID = "platform";
}

namespace ArkanoidGame
{
	Platform::Platform(const sf::Vector2f& position)
		: GameObject(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.PLATFORM_WIDTH, SETTINGS.PLATFORM_HEIGHT)
	{
		platformSpeed = SETTINGS.PLATFORM_SPEED;
		Vwidth = SETTINGS.PLATFORM_WIDTH;
	}

	void Platform::Update(float timeDelta)
	{
		if (BonusManager::IsSizeBonusActive()) {
			PlatformSizeCommand sizeCommand(*this);
			sizeCommand.Execute(*this);
		}
		else {
			SetSize(SETTINGS.PLATFORM_WIDTH);
		}
		if (BonusManager::IsSpeedBonusActive()) {
			PlatformSpeedCommand speedCommand(*this);
			speedCommand.Execute(*this);
		}
		else {
			SetSpeed(SETTINGS.PLATFORM_SPEED);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Move(-timeDelta * platformSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Move(timeDelta * platformSpeed);
		}
	}

	void Platform::Move(float speed)
	{
		auto position = sprite.getPosition();
		position.x = std::clamp(position.x + speed, Vwidth / 2.f, SETTINGS.SCREEN_WIDTH - Vwidth / 2.f);
		sprite.setPosition(position);
	}

	bool Platform::GetCollision(std::shared_ptr<Colladiable> collidable) const
	{
		auto ball = std::static_pointer_cast<Ball>(collidable);
		if (!ball) return false;

		auto sqr = [](float x) {
			return x * x;
		};
		const auto rect = sprite.getGlobalBounds();
		const auto ballPos = ball->GetPosition();
		if (ballPos.x < rect.left) {
			return sqr(ballPos.x - rect.left) + sqr(ballPos.y - rect.top) < sqr(SETTINGS.BALL_SIZE / 2.0);
		}

		if (ballPos.x > rect.left + rect.width) {
			return sqr(ballPos.x - rect.left - rect.width) + sqr(ballPos.y - rect.top) < sqr(SETTINGS.BALL_SIZE / 2.0);
		}

		return std::fabs(ballPos.y - rect.top) <= SETTINGS.BALL_SIZE / 2.0;
	}

	bool Platform::CheckCollision(std::shared_ptr<Colladiable> collidable) {
		auto ball = std::static_pointer_cast<Ball>(collidable);
		if (!ball)
			return false;

		if (GetCollision(ball)) {
			if (BonusManager::IsbounceDirectionBonusActive()) {
				// Устанавливаем случайный угол отскока между 75 и 105 градусами
				float randomAngle = random(75.0f, 105.0f);
				ball->ChangeAngle(randomAngle);
			}
			else {
				auto rect = GetRect();
				auto ballPosInOlatform = (ball->GetPosition().x - (rect.left + rect.width / 2)) / (rect.width / 2);
				ball->ChangeAngle(90 - 20 * ballPosInOlatform);
			}
			return true;
		}
		return false;
	}


	void Platform::SetSize(float width)
	{
		sprite.setScale(width / SETTINGS.PLATFORM_WIDTH, sprite.getScale().y);
		Vwidth = width;
	}

	void Platform::SetSpeed(float speed)
	{
		platformSpeed = speed;
	}

	void Platform::SetBounceDirection(float angle)
	{
	}

	float Platform::GetSpeed() const
	{
		return platformSpeed;
	}

	float Platform::GetWidth() const
	{
		return Vwidth;
	}


}