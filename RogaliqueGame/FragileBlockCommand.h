#pragma once
#include "ICommand.h"
#include "Block.h"

namespace RogaliqueGame {
    class FragileBlockCommand : public ICommand {
    public:
        void Execute(Platform& platform) override {}
        void Execute(Ball& ball) override {}
        void Execute(Block& block) override {
            block.OnHit(); 
        }
    };
}