#pragma once
#include "ICommand.h"
#include "Platform.h"


namespace ArkanoidGame
{
	class PlatformSpeedCommand : public ICommand
	{
    public:
        PlatformSpeedCommand(Platform& platform);
        void Execute(Block& block) override {}
        void Execute(Ball& ball) override {};
        void Execute(Platform& platform) override; 

    private:
        Platform& platform_;
	};

}

