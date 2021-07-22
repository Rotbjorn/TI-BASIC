#pragma once

#include "Bytecode.h"
#include "Op.h"

#include <cstdint>
#include <vector>

namespace TIBASIC::Compiler {

struct Assembler {
    std::vector<int8_t> m_bytes;
    std::vector<int> m_lines;

    TIBASIC::Opcode last_opcode;


    void write_op(TIBASIC::Opcode, int = -1);
    void write(int8_t, int = -1);
    void write_int16(int16_t, int = -1);
    void write_int32(int32_t, int = -1);

    TIBASIC::Bytecode get_bytecode();

};

}
