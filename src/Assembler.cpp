#include "Assembler.h"

#include <iostream>

namespace TIBASIC::Compiler {


int Assembler::add_constant(Value value) {
    m_constants.push_back(value);
    return m_constants.size() - 1;
}

void Assembler::write_op(TIBASIC::Opcode op, int line) {
    write((uint8_t)op, line);
    last_opcode = op;
}

void Assembler::write(uint8_t value, int line) {

    m_bytes.push_back(value);
    m_lines.push_back(line);
}

void Assembler::write_int16(uint16_t value, int line) {

    int8_t high = ((uint16_t) value >> 8) & 0xFF;
    int8_t low = (value & 0xFF);

    write(high, line);
    write(low, line);

}

void Assembler::write_int32(uint32_t value, int line) {

    int16_t high = ((uint32_t) value >> 16) & 0xFFFF;
    int16_t low = (value & 0xFFFF);


    write_int16(high, line);
    write_int16(low, line);
}


Bytecode* Assembler::get_bytecode() {
    Bytecode* bc = new Bytecode;

    size_t size = m_bytes.size();
    bc->m_size = size;

    bc->m_bytes = new uint8_t[size];

    std::copy(m_bytes.begin(), m_bytes.end(), bc->m_bytes);
    std::copy(m_constants.begin(), m_constants.end(), bc->m_constants);

    return bc;
}



}
