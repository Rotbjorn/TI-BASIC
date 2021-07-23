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

    TIBASIC::Opcode last_opcode;

    int add_constant(Value value);

    void write_op(TIBASIC::Opcode, int = -1);
    void write(uint8_t, int = -1);
    void write_int16(uint16_t, int = -1);
    void write_int32(uint32_t, int = -1);

    TIBASIC::Bytecode* get_bytecode();

};

}
