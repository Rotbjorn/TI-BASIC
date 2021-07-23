#include "Instruction.h"
#include <iostream>

#define DEF_INSTRUCTION(n) void INS_##n(TIBASIC::Runtime::VM* vm)

namespace TIBASIC {

static uint8_t read(uint8_t*& pc) {
    uint8_t value = *pc;
    pc++;
    return value;
}

static uint16_t read_int16(uint8_t*& pc) {
    uint16_t value = (read(pc) << 8);
    value |= read(pc);
    return value;
}

static uint32_t read_int32(uint8_t*& pc) {
    uint32_t value = (read_int16(pc) << 16);
    value |= read_int16(pc);
    return value;
}


#define __OPCODES(op) \
    INS_##op,

Instruction instructions[127] = {
    ENUMERATE_OPCODES(__OPCODES)
};

#undef __OPCODES


DEF_INSTRUCTION(CONSTANT) {
    uint8_t index = read(vm->pc);
    Value constant = vm->bytecode->m_constants[index];
    vm->stack.push(constant);
}

#define BINARY_OPERATION(op) \
    do {\
    double b = vm->stack.pop().number; \
    double a = vm->stack.pop().number; \
    vm->stack.push({a op b}); \
} while (0)


DEF_INSTRUCTION(ADD) {
    BINARY_OPERATION(+);
}

DEF_INSTRUCTION(SUBTRACT) {
    BINARY_OPERATION(-);
}

DEF_INSTRUCTION(MULTIPLY) {
    BINARY_OPERATION(*);
}

DEF_INSTRUCTION(DIVIDE) {
    BINARY_OPERATION(/);
}

#undef BINARY_OPERATION

DEF_INSTRUCTION(NEGATE) {
    vm->stack.push({-vm->stack.pop().number});
}


/*
DEF_INSTRUCTION(PRINT_STRING) {
    int8_t length = vm->stack.pop();
    char* string = new char[length + 2];
    string[length + 1] = 0;
    string[length--] = '\n';
    while (length >= 0) {
        string[length--] = (char)vm->stack.pop();
    }
    std::cout << string;
    delete[] string;
}
*/

DEF_INSTRUCTION(EXIT) {
    int8_t return_code = read(vm->pc);

    vm->exit_code = return_code;
}

}
