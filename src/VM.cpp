#include "VM.h"
#include "Instruction.h"

#include <iostream>

namespace TIBASIC::Runtime {

void Stack::push(int8_t value) {
    *stack_pointer++ = value;
}

void Stack::push_int16(int16_t value) {

    int8_t low = value & 0xFF;
    int8_t high = ((uint16_t)value >> 8) & 0xFF;

    push(low & 0xFF);
    push(high);
}

void Stack::push_int32(int32_t value) {

    int16_t low = (value) & 0x0000FFFF;
    int16_t high = ((uint32_t)value >> 16) & 0x0000FFFF;

    push_int16(low);
    push_int16(high);

}

uint8_t Stack::pop() {
    return *--stack_pointer;
}

uint16_t Stack::pop_int16() {
    int16_t value = 0x0000;

    value |= (uint8_t) pop();
    value = (uint16_t) value << 8;
    value |= (uint8_t) pop();

    return value;
}

uint32_t Stack::pop_int32() {
    int32_t value = 0x00000000;

    value |= pop_int16();
    value = (uint32_t) value << 16;
    value |= pop_int16();

    return value;
}

}

namespace TIBASIC::Runtime {

void VM::run_bytecode(Bytecode& bc){
    pc = bc.m_bytes;

    while(true) {
        uint8_t instruction = *pc;
        pc++;
        instructions[instruction](this);
        if (instruction == Opcode::EXIT) break;
    }
}

void VM::display_stack(const char* name) {
    uint8_t* first = stack.stack;
    int count = 1;

    if (first == stack.stack_pointer) {
        std::cout << "\n== " << name << " (Empty) ==\n\n";
        return;
    }

    std::cout << "\n== " << name << " ==\n" << std::hex;
    while(first < stack.stack_pointer) {
        std::cout << "[ " << (int)*first << " ] ";
        if (count % 4 == 0)
            std::cout << "\n";
        first++;
        count++;
    }
    std::cout << std::dec << "\n";
}

}
