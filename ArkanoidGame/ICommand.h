#pragma once

namespace ArkanoidGame {
    class Block; 
    class Platform;
    class Ball;

    class ICommand {
    public:
        virtual void Execute(Block& block) = 0; 
        virtual void Execute(Platform& platform) = 0;
        virtual void Execute(Ball& Ball) = 0;
        virtual ~ICommand() = default;
    };
}