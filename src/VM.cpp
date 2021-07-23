#include "VM.h"
#include "Instruction.h"

#include <iostream>

namespace TIBASIC::Runtime {

void Stack::push(Value value) {
    *stack_pointer++ = value;
}

Value Stack::pop() {
    return *--stack_pointer;
}

}

namespace TIBASIC::Runtime {

void VM::execute(Bytecode& bc){
    pc = bc.m_bytes;
    bytecode = &bc;

    while(true) {
        uint8_t instruction = *pc;
        pc++;
        instructions[instruction](this);
        if (instruction == Opcode::EXIT) break;
    }
}

void VM::display_stack(const char* name) {
    Value* first = stack.stack;
    int count = 1;

    if (first == stack.stack_pointer) {
        std::cout << "\n== " << name << " (Empty) ==\n\n";
        return;
    }

    std::cout << "\n== " << name << " ==\n" << std::hex;
    while(first < stack.stack_pointer) {
        std::cout << "[ ";
        first->print();
        std::cout << " ] ";
        if (count % 4 == 0)
            std::cout << "\n";
        first++;
        count++;
    }
    std::cout << std::dec << "\n";
}

}
