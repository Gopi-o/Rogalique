#pragma once
#include "ICommand.h"
#include "Platform.h"

namespace ArkanoidGame
{
	class PlatformBounceCommand : public ICommand
	{
	public:
		PlatformBounceCommand(Platform& platform);
		void Execute(Block& block) override {}
		void Execute(Ball& ball) override {};
		void Execute(Platform& platform) override;

	private:
		Platform& platform_;
	};
}

