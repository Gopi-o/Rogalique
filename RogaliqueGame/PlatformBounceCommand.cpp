#include "PlatformBounceCommand.h"


namespace RogaliqueGame
{
	PlatformBounceCommand::PlatformBounceCommand(Platform& platform)
		: platform_(platform)
	{
	}

	void PlatformBounceCommand::Execute(Platform& platform)
	{
		float angle = static_cast<float>((rand() % 360) * (3.14f / 180.0f)); 
		platform_.SetBounceDirection(angle);
	}

}
