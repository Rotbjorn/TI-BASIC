#pragma once

#include "Bytecode.h"
#include "Register.h"
#include "Stack.h"
#include <cstdint>

using PC = uint8_t*;

namespace TIBASIC::Runtime {


struct VM {
    Register reg;

    PC pc;

    Stack stack;

    Bytecode* bytecode;

    int32_t exit_code = 0;

    void execute(Bytecode& bc);

    void display_stack(const char*);
};

}
