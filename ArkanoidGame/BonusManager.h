#pragma once
namespace ArkanoidGame
{
	class BonusManager
	{
    public:
        static void ActivateFragileBonus(float duration);
        static void ActivateSizeBonus(float duration);
        static void ActivateSpeedBonus(float duration);
        static void ActivateBounceDirectionBonus(float duration);
        static void ActivateFastBallBonus(float duration);
        static bool IsFragileBonusActive();
        static bool IsSizeBonusActive();
        static bool IsSpeedBonusActive();
        static bool IsbounceDirectionBonusActive();
        static bool IsFastBallActive();

        static void Update(float deltaTime);

    private:
        static bool fragileBonusActive;
        static float fragileBonusDuration;
        static bool sizeBonusActive;
        static bool speedBonusActive;
        static bool bounceDirectionBonusActive;
        static bool FastBallActive;

        static float sizeBonusDuration;
        static float speedBonusDuration;
        static float timeRemaining;
        static float bounceDirectionBonusDuration;
        static float FastBallDuration;
	};

}

