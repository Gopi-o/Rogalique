#pragma once
#include "Components/Component.h"
#include "Components/Render/SpriteRenderer/SpriteRendererComponent.h"
#include "Components/Physics/Transform/TransformComponent.h"
#include <functional>
#include <vector>

namespace Engine
{
	class EffectComponent : public Component
	{
	public:
		EffectComponent(GameObject* gameObject)
			: Component(gameObject) {}

		struct Effect
		{
			float duration;
			float elapsedTime;
			bool isActive;

			std::function<void(float)> updateTransform;
			std::function<void(float)> updateColor;
			std::function<void()> onComplete;
		};

		void AddEffect(const Effect& effect)
		{
			effects.push_back(effect);
		}

		void AddHitEffect(float duration = 0.2f)
		{
			auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
			auto transform = gameObject->GetComponent<TransformComponent>();

			if (!spriteRenderer || !transform)
				return;

			Effect hitEffect;
			hitEffect.duration = duration;
			hitEffect.elapsedTime = 0.0f;
			hitEffect.isActive = true;

			auto initialScale = transform->GetWorldScale();
			auto initialPosition = transform->GetWorldPosition();
			auto initialColor = spriteRenderer->GetSprite()->getColor();

			hitEffect.updateTransform = [transform, initialPosition, initialScale](float progress) {
				// Уменьшаем размер только в начале атаки
				float scaleFactor = 1.0f;
				if (progress < 0.5f)
				{
					scaleFactor = 1.0f - (0.2f * (progress * 2.0f)); // Уменьшаем размер в первой половине
				}
				else
				{
					scaleFactor = 0.8f + (0.2f * ((progress - 0.5f) * 2.0f)); // Возвращаем размер во второй половине
				}

				transform->SetWorldScale({ initialScale.x * scaleFactor,
					initialScale.y * scaleFactor });

				// Небольшой отскок
				float bounceHeight = 5.0f * std::sin(progress * 3.14159);
				transform->SetWorldPosition({ initialPosition.x,
					initialPosition.y - bounceHeight });
			};

			hitEffect.onComplete = [transform, initialPosition, initialScale]() {
				// Гарантируем, что в конце эффекта размер и позиция точно вернутся к исходным
				transform->SetWorldPosition(initialPosition);
				transform->SetWorldScale(initialScale);
			};

			AddEffect(hitEffect);
		}

		void Update(float deltaTime) override
		{
			for (auto& effect : effects)
			{
				if (!effect.isActive)
					continue;

				effect.elapsedTime += deltaTime;
				float progress = effect.elapsedTime / effect.duration;

				if (progress >= 1.0f)
				{
					effect.isActive = false;
					if (effect.onComplete)
						effect.onComplete();
					continue;
				}

				if (effect.updateTransform)
					effect.updateTransform(progress);
				if (effect.updateColor)
					effect.updateColor(progress);
			}

			effects.erase(
				std::remove_if(effects.begin(), effects.end(),
					[](const Effect& effect) { return !effect.isActive; }),
				effects.end());
		}

		void Render() override {};

	private:
		std::vector<Effect> effects;
	};
} // namespace Engine
