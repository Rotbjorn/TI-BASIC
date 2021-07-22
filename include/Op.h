#pragma once

#include <cstdint>

namespace TIBASIC {

#define ENUMERATE_OPCODES(O) \
    O(ADD_INT8) \
    O(ADD_INT16) \
    O(ADD_INT32) \
    O(SUB_INT8) \
    O(SUB_INT16) \
    O(SUB_INT32) \
    O(MULTIPLY) \
    O(PRINT_INT8) \
    O(PRINT_INT16) \
    O(PRINT_INT32) \
    O(PRINT_STRING) \
    O(PUSH_INT8) \
    O(PUSH_INT16) \
    O(PUSH_INT32) \
    O(PUSH_REG) \
    O(STORE_INT8) \
    O(STORE_INT16) \
    O(STORE_INT32) \
    O(EQUALS_INT8) \
    O(EQUALS_INT16) \
    O(EQUALS_INT32) \
    O(EXIT)


enum Opcode : int8_t {
#define __OPCODES(op) \
    op,

    ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES
};

const char* opcode_to_string(Opcode);

}
