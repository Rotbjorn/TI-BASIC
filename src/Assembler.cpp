#include "Assembler.h"

namespace TIBASIC::Compiler {


void Assembler::write_byte(uint8_t value, int line) {
    m_bytes.push_back(value);
    m_lines.push_back(line);
}

void Assembler::write(uint8_t value) {
    write_byte(value, m_line);
}

void Assembler::write_int16(uint16_t value) {
    write((value >> 8) & 0xFF);
    write(value & 0xFF);
}

void Assembler::write_op(TIBASIC::Opcode op) {
    write((uint8_t)op);
    last_opcode = op;
}

size_t Assembler::write_jump(Opcode op) {
    write_op(op);
    write_int16(0);
    return m_bytes.size() - 2; // points to the high of the int16
}

void Assembler::write_jump_back(size_t loop_start) {
    write_op(Opcode::JUMP_BACK);

    uint16_t offset = m_bytes.size() - loop_start + 2;

    write_int16(offset);
}

void Assembler::fix_jump(size_t offset) {
    uint16_t jump = m_bytes.size() - 2 - offset;
    rewrite_int16(offset, jump);
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

void Assembler::rewrite(size_t index, uint8_t value) {
    m_bytes.at(index) = value;
}

void Assembler::rewrite_int16(size_t index, uint16_t value) {
    rewrite(index, (value >> 8) & 0xFF);
    rewrite(index + 1, value & 0xFF);
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
