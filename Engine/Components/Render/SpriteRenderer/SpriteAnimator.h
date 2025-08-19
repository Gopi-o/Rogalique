// Engine/Components/Render/SpriteRenderer/SpriteAnimator.h
// Engine/Components/Render/SpriteRenderer/SpriteAnimatorComponent.h
#pragma once
#include "Components/Component.h"
#include "Components/Render/SpriteRenderer/SpriteRendererComponent.h"
#include "Components/Physics/Rigidbody/RigidbodyComponent.h"
#include <map>
#include <string>

namespace Engine
{
	class SpriteAnimator : public Component
	{
	public:
		enum class State
		{
			Idle,
			Move
		};
		enum class Dir
		{
			Down,
			Left,
			Right,
			Up
		};

		SpriteAnimator(GameObject* go);
		void Update(float dt) override;
		void Render() override {}

		void SetSpriteSheet(const std::string& textureKey, int tileW, int tileH);
		void SetAnimation(State st, Dir dir, int row, int startCol, int frameCount, float fps);
		void Play(State st);
		void SetAutoByVelocity(bool enable) { autoByVelocity = enable; }
		void SetIdleThreshold(float v) { idleThreshold = v; }
		void SetDirectionFromInput(const Vector2Df& input);

	private:
		struct Anim
		{
			int row = 0, start = 0, count = 1;
			float fps = 6.f;
		};
		SpriteRendererComponent* renderer = nullptr;
		RigidbodyComponent* body = nullptr;
		const sf::Texture* texture = nullptr;
		int tileW = 32, tileH = 32;

		std::map<std::pair<State, Dir>, Anim> anims;
		State currentState = State::Idle;
		Dir currentDir = Dir::Down;
		float timeAcc = 0.f;
		int curFrame = 0;
		bool autoByVelocity = true;
		float idleThreshold = 0.05f;

		Vector2Df filteredV = { 0.f, 0.f };
		float filterAlpha = 0.25f; // сглаживание скорости
		float dirDeadZone = 0.05f; // мёртвая зона
		float axisBis = 1.15f;	   // приоритет последней оси
		Dir lastDir = Dir::Down;

		void applyFrame();
		void updateDirFromVelocity(const Vector2Df& v);
	};
} // namespace Engine