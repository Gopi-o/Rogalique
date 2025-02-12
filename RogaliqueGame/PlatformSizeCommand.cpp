#include "PlatformSizeCommand.h"
#include "GameSettings.h"

namespace RogaliqueGame
{
	PlatformSizeCommand::PlatformSizeCommand(Platform& platform)
		: platform_(platform)
	{
	}

	void PlatformSizeCommand::Execute(Platform& platform)
	{
		auto newSize = SETTINGS.PLATFORM_BONUS_WIDTH;
		platform.SetSize(newSize);
	}

}

