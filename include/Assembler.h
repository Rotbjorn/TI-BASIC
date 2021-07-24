#pragma once

#include "Bytecode.h"
#include "Op.h"

#include <cstdint>
#include <vector>

namespace TIBASIC::Compiler {

struct Assembler {
    std::vector<uint8_t> m_bytes;
    std::vector<Value> m_constants;
    std::vector<int> m_lines;

    int m_line;

    TIBASIC::Opcode last_opcode;

    void write_byte(uint8_t, int);

    void write(uint8_t);
    void write_op(TIBASIC::Opcode);
    int add_constant(Value);
    void write_constant(Value);

    TIBASIC::Bytecode* get_bytecode();

};

}
