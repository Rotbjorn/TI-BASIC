
#pragma once

#include <cstdint>

namespace TIBASIC {

#define ENUMERATE_OPCODES(O) \
    O(CONSTANT) \
    O(ADD) \
    O(SUBTRACT) \
    O(MULTIPLY) \
    O(DIVIDE) \
    O(PUSH_TRUE) \
    O(PUSH_FALSE) \
    O(EQUAL) \
    O(NOT_EQUAL) \
    O(GREATER) \
    O(LESS) \
    O(GREATER_OR_EQUAL) \
    O(LESS_OR_EQUAL) \
    O(NEGATE) \
    O(PRINT) \
    O(STORE_NUMBER) \
    O(GET_NUMBER) \
    O(POP) \
    O(EXIT)


enum Opcode : int8_t {
#define __OPCODES(op) \
    op,

    ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES
};

const char* opcode_to_string(Opcode);

}
