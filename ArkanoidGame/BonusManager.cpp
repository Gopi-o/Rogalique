#include "BonusManager.h"

namespace ArkanoidGame
{

	bool BonusManager::fragileBonusActive = false;
	float BonusManager::fragileBonusDuration = 0.0f;
	bool BonusManager::sizeBonusActive = false;
	float BonusManager::sizeBonusDuration = 0.0f;
	bool BonusManager::speedBonusActive = false;
	float BonusManager::speedBonusDuration = 0.0f;
	bool BonusManager::bounceDirectionBonusActive = false;
	float BonusManager::bounceDirectionBonusDuration = 0.0f;
	bool BonusManager::FastBallActive = false;
	float BonusManager::FastBallDuration = 0.0f;

	void BonusManager::ActivateFragileBonus(float duration)
	{
		fragileBonusActive = true;
		fragileBonusDuration = duration;
	}

	void BonusManager::ActivateSizeBonus(float duration)
	{
		sizeBonusActive = true;
		sizeBonusDuration = duration;
	}

	void BonusManager::ActivateSpeedBonus(float duration)
	{
		speedBonusActive = true;
		speedBonusDuration = duration;
	}

	void BonusManager::ActivateBounceDirectionBonus(float duration)
	{
		bounceDirectionBonusActive = true;
		bounceDirectionBonusDuration = duration;
	}

	void BonusManager::ActivateFastBallBonus(float duration)
	{
		FastBallActive = true;
		FastBallDuration = duration;
	}

	bool BonusManager::IsFragileBonusActive()
	{
		return fragileBonusActive;
	}

	bool BonusManager::IsSizeBonusActive()
	{
		return sizeBonusActive;
	}

	bool BonusManager::IsSpeedBonusActive()
	{
		return speedBonusActive;
	}

	bool BonusManager::IsbounceDirectionBonusActive()
	{
		return bounceDirectionBonusActive;
	}

	bool BonusManager::IsFastBallActive()
	{
		return FastBallActive;
	}

	void BonusManager::Update(float deltaTime)
	{
		if (fragileBonusActive) {
			fragileBonusDuration -= deltaTime;
			if (fragileBonusDuration <= 0) {
				fragileBonusActive = false; 
			}
		}
		if (sizeBonusActive) {
			sizeBonusDuration -= deltaTime;
			if (sizeBonusDuration <= 0) {
				sizeBonusActive = false;
			}
		}
		if (speedBonusActive) {
			speedBonusDuration -= deltaTime;
			if (speedBonusDuration <= 0) {
				speedBonusActive = false;
			}
		}
		if (bounceDirectionBonusActive) {
			bounceDirectionBonusDuration -= deltaTime;
			if (bounceDirectionBonusDuration <= 0) {
				bounceDirectionBonusActive = false;
			}
		}
		if (FastBallActive) {
			FastBallDuration -= deltaTime;
			if (FastBallDuration <= 0) {
				FastBallActive = false;
			}
		}
	}
}

