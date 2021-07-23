
#pragma once

#include <cstdint>

namespace TIBASIC {

#define ENUMERATE_OPCODES(O) \
    O(CONSTANT) \
    O(EXIT)


enum Opcode : int8_t {
#define __OPCODES(op) \
    op,

    ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES
};

const char* opcode_to_string(Opcode);

}
