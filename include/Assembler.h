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
    void write_int16(uint16_t);

    void write_op(Opcode);

    size_t write_jump(Opcode);
    void write_jump_back(size_t);
    void fix_jump(size_t offset);

    int add_constant(Value);
    void write_constant(Value);

    void rewrite(size_t index, uint8_t);
    void rewrite_int16(size_t index, uint16_t);

    TIBASIC::Bytecode* get_bytecode();

};

}
