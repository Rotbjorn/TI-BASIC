#pragma once

#include "Value.h"

namespace TIBASIC::Runtime {

struct Stack {
    Value stack[512];
    Value* stack_pointer;

    void push(Value);
    Value pop();

    Value peek(int = 0);

    Stack() : stack_pointer { stack } {}
};

}
