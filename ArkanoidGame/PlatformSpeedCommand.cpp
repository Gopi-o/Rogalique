#include "PlatformSpeedCommand.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
	PlatformSpeedCommand::PlatformSpeedCommand(Platform& platform)
		: platform_(platform)
	{
	}

	void PlatformSpeedCommand::Execute(Platform& platform)
	{
		platform.SetSpeed(SETTINGS.PLATFORM_BONUS_SPEED);
	}



}

