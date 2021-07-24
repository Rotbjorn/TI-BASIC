#include "Assembler.h"

#include <iostream>

namespace TIBASIC::Compiler {


void Assembler::write_byte(uint8_t value, int line) {
    m_bytes.push_back(value);
    m_lines.push_back(line);
}


void Assembler::write_op(TIBASIC::Opcode op) {
    write((uint8_t)op);
    last_opcode = op;
}

void Assembler::write(uint8_t value) {
    write_byte(value, m_line);
}

int Assembler::add_constant(Value value) {
    m_constants.push_back(value);
    return m_constants.size() - 1;
}

void Assembler::write_constant(Value value) {
    write_op(Opcode::CONSTANT);
    int index = add_constant(value);
    write(index);
}

Bytecode* Assembler::get_bytecode() {
    Bytecode* bc = new Bytecode;

    size_t size = m_bytes.size();
    bc->m_size = size;

    bc->m_constant_size = m_constants.size();

    bc->m_bytes = new uint8_t[size];

    std::copy(m_bytes.begin(), m_bytes.end(), bc->m_bytes);
    std::copy(m_constants.begin(), m_constants.end(), bc->m_constants);

    return bc;
}

}
