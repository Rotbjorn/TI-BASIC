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




DEF_INSTRUCTION(ADD_INT8) {
    uint8_t b = vm->stack.pop();
    uint8_t a = vm->stack.pop();
    vm->stack.push(a + b);
}

DEF_INSTRUCTION(ADD_INT16) {
    uint16_t b = vm->stack.pop_int16();
    uint16_t a = vm->stack.pop_int16();
    vm->stack.push_int16(a + b);
}

DEF_INSTRUCTION(ADD_INT32) {
    uint32_t b = vm->stack.pop_int32();
    uint32_t a = vm->stack.pop_int32();
    vm->stack.push_int32(a + b);
}

DEF_INSTRUCTION(SUB_INT8) {
    uint8_t b = vm->stack.pop();
    uint8_t a = vm->stack.pop();
    vm->stack.push(a - b);
}

DEF_INSTRUCTION(SUB_INT16) {
    uint16_t b = vm->stack.pop_int16();
    uint16_t a = vm->stack.pop_int16();
    vm->stack.push_int16(a - b);
}

DEF_INSTRUCTION(SUB_INT32) {
    uint32_t b = vm->stack.pop_int32();
    uint32_t a = vm->stack.pop_int32();
    vm->stack.push_int32(a - b);
}

DEF_INSTRUCTION(MULTIPLY) {
    uint8_t b = vm->stack.pop();
    uint8_t a = vm->stack.pop();
    vm->stack.push(a * b);
}

DEF_INSTRUCTION(PRINT_INT8) {
    int32_t value = vm->stack.pop();
    std::cout << value << std::endl;
}

DEF_INSTRUCTION(PRINT_INT16) {
    int32_t value = vm->stack.pop_int16();
    std::cout << value << std::endl;
}

DEF_INSTRUCTION(PRINT_INT32) {
    int32_t value = vm->stack.pop_int32();
    std::cout << value << std::endl;
}

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

DEF_INSTRUCTION(PUSH_INT8) {
    uint8_t value = read(vm->pc);

    vm->stack.push(value);
}

// PUSH 00 00
DEF_INSTRUCTION(PUSH_INT16) {
    uint16_t value = read_int16(vm->pc);

    vm->stack.push_int16(value);
}

// PUSH 12 34 56 78
DEF_INSTRUCTION(PUSH_INT32) {
    uint32_t value = read_int32(vm->pc);



    vm->stack.push_int32(value);
}

DEF_INSTRUCTION(PUSH_REG) {
    uint8_t reg_index = read(vm->pc);

    vm->stack.push_int32(vm->reg.int_registers[reg_index]);
}

DEF_INSTRUCTION(STORE_INT8) {
    uint8_t reg_index = read(vm->pc);

    vm->reg.int_registers[reg_index] = vm->stack.pop();
}

DEF_INSTRUCTION(STORE_INT16) {
    uint8_t reg_index = read(vm->pc);

    int16_t COOL_VALUE = vm->stack.pop_int16();
    vm->reg.int_registers[reg_index] = COOL_VALUE;
}

DEF_INSTRUCTION(STORE_INT32) {
    uint8_t reg_index = read(vm->pc);

    vm->reg.int_registers[reg_index] = vm->stack.pop_int32();
}

DEF_INSTRUCTION(EQUALS_INT8) {
    vm->stack.push(vm->stack.pop() == vm->stack.pop());
}

DEF_INSTRUCTION(EQUALS_INT16) {
    vm->stack.push(vm->stack.pop_int16() == vm->stack.pop_int16());
}

DEF_INSTRUCTION(EQUALS_INT32) {
    vm->stack.push(vm->stack.pop_int32() == vm->stack.pop_int32());
}

DEF_INSTRUCTION(EXIT) {
    int8_t return_code = read(vm->pc);

    vm->exit_code = return_code;
}

}
