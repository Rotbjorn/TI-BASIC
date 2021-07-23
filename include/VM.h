#pragma once

#include <cstdint>
#include "Bytecode.h"
#include "Register.h"

using PC = uint8_t*;

namespace TIBASIC::Runtime {

struct Stack {
    Value stack[512];
    Value* stack_pointer;

    void push(Value);

    Value pop();

    Stack() : stack_pointer { stack } {}
};

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
