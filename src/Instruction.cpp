#include "Instruction.h"

#include <cmath>
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
    if (!vm->stack.peek(0).is_double() || !vm->stack.peek(1).is_double()) {\
    std::cout << "Operands are not doubles!\n"; \
} \
    double b = vm->stack.pop().as.double_number; \
    double a = vm->stack.pop().as.double_number; \
    vm->stack.push(Value(a op b)); \
} while (0)


DEF_INSTRUCTION(ADD) {
    if(vm->stack.peek(0).is_string() && vm->stack.peek(1).is_string()) {
        String b = vm->stack.pop().as.string;
        String a = vm->stack.pop().as.string;

        a.concat(b);

        vm->stack.push(Value(a));

    } else if(vm->stack.peek(0).is_double() && vm->stack.peek(1).is_double()) {
        double b = vm->stack.pop().as.double_number;
        double a = vm->stack.pop().as.double_number;
        vm->stack.push(Value(a + b));
    } else {
        std::cerr << "Can't add those two values!!!\n";
    }
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

DEF_INSTRUCTION(POWER) {
    double exponent = vm->stack.pop().as.double_number;
    double base = vm->stack.pop().as.double_number;

    double result = std::pow(base, exponent);
    vm->stack.push(Value(result));
}

DEF_INSTRUCTION(PUSH_TRUE) {
    vm->stack.push(Value(true));
}

DEF_INSTRUCTION(PUSH_FALSE) {
    vm->stack.push(Value(false));
}

DEF_INSTRUCTION(EQUAL) {
    Value rhs = vm->stack.pop();
    Value lhs = vm->stack.pop();

    vm->stack.push(Value(lhs == rhs));
}

DEF_INSTRUCTION(NOT_EQUAL) {
    Value rhs = vm->stack.pop();
    Value lhs = vm->stack.pop();

    vm->stack.push(Value(lhs != rhs));
}

DEF_INSTRUCTION(GREATER) {
    BINARY_OPERATION(>);
}

DEF_INSTRUCTION(GREATER_OR_EQUAL) {
    BINARY_OPERATION(>=);
}

DEF_INSTRUCTION(LESS) {
    BINARY_OPERATION(<);
}

DEF_INSTRUCTION(LESS_OR_EQUAL) {
    BINARY_OPERATION(<=);
}

DEF_INSTRUCTION(NEGATE) {
    vm->stack.push(Value(-(vm->stack.pop().as.double_number)));
}

DEF_INSTRUCTION(PRINT) {
    vm->stack.pop().print();
    std::cout << "\n";
}

DEF_INSTRUCTION(INPUT) {
    std::string test;
    getline(std::cin, test);
    vm->stack.push(Value(std::stod(test)));
}

DEF_INSTRUCTION(STORE_NUMBER) {
    uint8_t reg_index = read(vm->pc);
    // TODO: Assert that popped value is int or double

    Value value = vm->stack.pop();

    if(!value.is_double()) {
        std::cerr << "Can't store non-number value in number register!\n";
    } else {
        vm->reg.numbers[reg_index] = value.as.double_number;
    }

}

DEF_INSTRUCTION(GET_NUMBER) {
    uint8_t reg_index = read(vm->pc);
    vm->stack.push(Value(vm->reg.numbers[reg_index]));
}

DEF_INSTRUCTION(STORE_STRING) {
    uint8_t reg_index = read(vm->pc);

    Value value = vm->stack.pop();

    if(!value.is_string()) {
        std::cerr << "Can't store non-string value in string register!\n";
    } else {
        vm->reg.strings[reg_index] = value.as.string;
    }
}

DEF_INSTRUCTION(GET_STRING) {
    uint8_t reg_index = read(vm->pc);

    vm->stack.push(Value(vm->reg.strings[reg_index]));
}

DEF_INSTRUCTION(POP) {
    vm->stack.pop();
}

DEF_INSTRUCTION(JUMP_IF_TRUE) {

    uint16_t offset = read_int16(vm->pc);

    if(vm->stack.peek().is_double()) {
        if(vm->stack.peek().as.double_number != 0)
            vm->pc += offset;
    } else if(vm->stack.peek().as.boolean) {
        vm->pc += offset;
    }


}


DEF_INSTRUCTION(JUMP_IF_FALSE) {

    uint16_t offset = read_int16(vm->pc);

    if(vm->stack.peek().is_double()) {
        if(vm->stack.peek().as.double_number == 0)
            vm->pc += offset;
    } else if(!vm->stack.peek().as.boolean) {
        vm->pc += offset;
    }
}

DEF_INSTRUCTION(JUMP) {
    uint16_t offset = read_int16(vm->pc);
    vm->pc += offset;
}

DEF_INSTRUCTION(JUMP_BACK) {
    uint16_t offset = read_int16(vm->pc);
    vm->pc -= offset;
}

DEF_INSTRUCTION(EXIT) {
    int8_t return_code = read(vm->pc);

    vm->exit_code = return_code;
}

}
