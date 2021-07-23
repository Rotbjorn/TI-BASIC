#include "Stack.h"

namespace TIBASIC::Runtime {

void Stack::push(Value value) {
    *stack_pointer++ = value;
}

Value Stack::pop() {
    return *--stack_pointer;
}

Value Stack::peek(int offset) {
    return stack_pointer[-1 - offset];
}

}
