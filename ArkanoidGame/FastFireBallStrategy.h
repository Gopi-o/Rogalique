#pragma once
#include "GameSettings.h"
#include "ICommand.h"
#include "BonusManager.h"
#include "Ball.h" 


namespace ArkanoidGame
{
    class FastFireBallStrategy : public ICommand
    {
    public:
        FastFireBallStrategy(Ball& ball);
        void Execute(Block& block) override {};
        void Execute(Platform& platform) override {}
        void Execute(Ball& ball) override;

    private:
        Ball& ball_;

    };

}