#include "pch.h"
#include "SpriteAnimator.h"
#include <Systems/Resource/ResourceSystem.h>
#include <cmath>
#include <Systems/Logger.h>

namespace Engine
{
	SpriteAnimator::SpriteAnimator(GameObject* go)
		: Component(go)
	{
		renderer = go->GetComponent<SpriteRendererComponent>();
		body = go->GetComponent<RigidbodyComponent>();
	}

	void SpriteAnimator::SetSpriteSheet(const std::string& textureKey, int w, int h)
	{
		tileW = w;
		tileH = h;
		auto tex = ResourceSystem::Instance()->GetTextureShared(textureKey);
		texture = tex;
		if (renderer && texture)
		{
			renderer->SetTexture(*texture);
			// выставим первый кадр (Idle/Down, колонка 0) до масштабирования
			const_cast<sf::Sprite*>(renderer->GetSprite())->setTextureRect(sf::IntRect(0, 0, tileW, tileH));
			// origin по тайлу
			const_cast<sf::Sprite*>(renderer->GetSprite())->setOrigin({ 0.5f * tileW, 0.5f * tileH });
			// теперь правильное масштабирование
			renderer->SetPixelSize(tileW, tileH);
		}
		applyFrame();
	}

	void SpriteAnimator::SetAnimation(State st, Dir dir, int row, int startCol, int frameCount, float fps)
	{
		anims[{ st, dir }] = Anim{ row, startCol, frameCount, fps };
	}

	void SpriteAnimator::Play(State st)
	{
		if (currentState != st)
		{
			currentState = st;
			curFrame = 0;
			timeAcc = 0.f;
			applyFrame();
		}
	}

	void SpriteAnimator::SetDirectionFromInput(const Vector2Df& in)
	{
		if (in.x == 0.f && in.y == 0.f)
		{
			Play(State::Idle);
			return;
		}

		if (std::abs(in.x) >= std::abs(in.y))
		{
			currentDir = (in.x >= 0.f) ? Dir::Right : Dir::Left;
		}
		else
		{
			currentDir = (in.y >= 0.f) ? Dir::Up : Dir::Down;
		}
		lastDir = currentDir;

		// включаем анимацию движения
		Play(State::Move);
	}

	void SpriteAnimator::Update(float dt)
	{
		if (!renderer || !texture)
			return;

		if (autoByVelocity && body)
		{
			auto v = body->GetLinearVelocity();
			float speed2 = v.x * v.x + v.y * v.y;
			if (speed2 > idleThreshold * idleThreshold)
			{
				updateDirFromVelocity(v);
				Play(State::Move);
			}
			else
			{
				Play(State::Idle);
			}
		}

		auto it = anims.find({ currentState, currentDir });
		if (it == anims.end())
			return;

		const Anim& a = it->second;
		timeAcc += dt;
		if (a.fps > 0.f)
		{
			float frameTime = 1.f / a.fps;
			while (timeAcc >= frameTime)
			{
				timeAcc -= frameTime;
				curFrame = (curFrame + 1) % std::max(1, a.count);
				applyFrame();
			}
		}
	}

	void SpriteAnimator::applyFrame()
	{
		if (!renderer || !texture)
			return;
		auto it = anims.find({ currentState, currentDir });
		if (it == anims.end())
			return;
		const Anim& a = it->second;

		int col = a.start + curFrame;
		sf::IntRect rect(col * tileW, a.row * tileH, tileW, tileH);
		// Достаём спрайт и выставляем текстурный прямоугольник
		const_cast<sf::Sprite*>(renderer->GetSprite())->setTextureRect(rect);
	}

	void SpriteAnimator::updateDirFromVelocity(const Vector2Df& v)
	{
		filteredV.x += (v.x - filteredV.x) * filterAlpha;
		filteredV.y += (v.y - filteredV.y) * filterAlpha;

		if (std::abs(filteredV.x) < dirDeadZone && std::abs(filteredV.y) < dirDeadZone)
		{
			currentDir = lastDir;
			return;
		}

		// переведём в «игровую» систему (Up при +Y ввода): инвертировать НЕ нужно, если уже используете ввод
		Dir dir;
		if (std::abs(filteredV.x) * axisBis > std::abs(filteredV.y))
		{
			dir = (filteredV.x >= 0.f) ? Dir::Right : Dir::Left;
		}
		else
		{
			dir = (filteredV.y >= 0.f) ? Dir::Up : Dir::Down;
		}
		currentDir = lastDir = dir;
	}
} // namespace Engine