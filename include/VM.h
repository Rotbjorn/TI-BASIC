#pragma once

#include <cstdint>
#include "Bytecode.h"
#include "Register.h"

using PC = uint8_t*;

namespace TIBASIC::Runtime {

struct Stack {
    uint8_t stack[512];
    uint8_t* stack_pointer;

    void push(int8_t);
    void push_int16(int16_t);
    void push_int32(int32_t);

    uint8_t pop();
    uint16_t pop_int16();
    uint32_t pop_int32();

    Stack() : stack_pointer { stack } {}
};

struct VM {
    Register reg;

    PC pc;

    Stack stack;

    int32_t exit_code = 0;

    void run_bytecode(Bytecode& bc);

    void display_stack(const char*);
};

}
