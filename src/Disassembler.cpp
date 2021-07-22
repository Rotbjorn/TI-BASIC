#include "Disassembler.h"

#include <iostream>
#include <iomanip>

#define DISASSEMBLE_INSTRUCTION(n) static void DIS_##n(uint8_t*& pc)


namespace TIBASIC::Disassembler {


using DisassembleInstruction = void (*)(uint8_t*&);
extern DisassembleInstruction disassemble_instructions[127];


#define __OPCODES(op) \
    static void DIS_##op(uint8_t*&);

ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES



#define __OPCODES(op)\
    DIS_##op,

DisassembleInstruction disassemble_instructions[127] = {
    ENUMERATE_OPCODES(__OPCODES)
};

#undef __OPCODES

void disassemble_bytecode(TIBASIC::Bytecode bc, const char* name) {
    std::cout << "== " << name << " ==\n";

    uint8_t* pc = bc.m_bytes;

    while(true) {
        size_t index = pc - bc.m_bytes;

        std::cout << std::right << std::setfill('0') << std::setw(4) << index << "  ";

        int8_t current_instruction = *pc;
        pc++;
        disassemble_instructions[current_instruction](pc);

        if(current_instruction == Opcode::EXIT) break;
    }

}

void disassemble_bytecode(TIBASIC::Bytecode bc, int* lines, const char* name) {
    std::cout << "== " << name << " ==\n";

    uint8_t* pc = bc.m_bytes;

    while(true) {
        size_t index = pc - bc.m_bytes;
        std::cout << std::right << std::setfill('0') << std::setw(4) << (pc - bc.m_bytes) << "  ";

        if (index > 0 && lines[index] == lines[index - 1]) {
            std::cout << "  | ";
        } else {
            std::cout << std::setfill(' ') << std::setw(3) << lines[index] << " ";
        }

        int8_t current_instruction = *pc;
        pc++;
        disassemble_instructions[current_instruction](pc);

        if(current_instruction == Opcode::EXIT) break;
    }
}

};

namespace TIBASIC::Disassembler {

static uint8_t read(uint8_t*& pc) {
    uint8_t value = *pc;
    pc++;
    return value;
}

static uint16_t read_int16(uint8_t*& pc) {
    uint16_t value = ((uint8_t)read(pc) << 8);
    value |= (uint8_t) read(pc);
    return value;
}

static uint32_t read_int32(uint8_t*& pc) {
    uint32_t value = ((uint16_t) read_int16(pc) << 16);
    value |= (uint16_t) read_int16(pc);
    return value;
}


DISASSEMBLE_INSTRUCTION(ADD_INT8) {
    std::cout << "ADD_INT8" << std::endl;
}

DISASSEMBLE_INSTRUCTION(ADD_INT16) {
    std::cout << "ADD_INT16" << std::endl;
}

DISASSEMBLE_INSTRUCTION(ADD_INT32) {
    std::cout << "ADD_INT32" << std::endl;
}

DISASSEMBLE_INSTRUCTION(SUB_INT8) {
    std::cout << "SUB_INT8" << std::endl;
}

DISASSEMBLE_INSTRUCTION(SUB_INT16) {
    std::cout << "SUB_INT16" << std::endl;
}

DISASSEMBLE_INSTRUCTION(SUB_INT32) {
    std::cout << "SUB_INT32" << std::endl;
}

DISASSEMBLE_INSTRUCTION(MULTIPLY) {
    std::cout << "MULTIPLY" << std::endl;
}

DISASSEMBLE_INSTRUCTION(PRINT_INT8) {
    std::cout << "PRINT_INT8" << std::endl;
}

DISASSEMBLE_INSTRUCTION(PRINT_INT16) {
    std::cout << "PRINT_INT16" << std::endl;
}

DISASSEMBLE_INSTRUCTION(PRINT_INT32) {
    std::cout << "PRINT_INT32" << std::endl;
}

DISASSEMBLE_INSTRUCTION(PRINT_STRING) {
    std::cout << "PRINT_STRING" << std::endl;
}

DISASSEMBLE_INSTRUCTION(PUSH_INT8) {
    uint8_t value = read(pc);
    std::cout << "PUSH_INT8 " << (int)value << std::endl;
}


DISASSEMBLE_INSTRUCTION(PUSH_INT16) {
    uint16_t value  = read_int16(pc);
    std::cout << "PUSH_INT16 " << value << std::endl;
}

DISASSEMBLE_INSTRUCTION(PUSH_INT32) {
    uint32_t value = read_int32(pc);
    std::cout << "PUSH_INT32 " << value << std::endl;
}

DISASSEMBLE_INSTRUCTION(PUSH_REG) {
    uint8_t register_index = read(pc);
    std::cout << "PUSH_REG " << (int)register_index << " ('" << (char)(register_index + 'A') << "')" << std::endl;
}

DISASSEMBLE_INSTRUCTION(STORE_INT8) {
    uint8_t register_index = read(pc);
    std::cout << "STORE_INT8 " << (int)register_index << " ('" << (char)(register_index + 'A') << "')" << std::endl;
}

DISASSEMBLE_INSTRUCTION(STORE_INT16) {
    uint8_t register_index = read(pc);
    std::cout << "STORE_INT16 " << (int)register_index << " ('" << (char)(register_index + 'A') << "')" << std::endl;
}

DISASSEMBLE_INSTRUCTION(STORE_INT32) {
    uint8_t register_index = read(pc);
    std::cout << "STORE_INT32 " << (int)register_index << " ('" << (char)(register_index + 'A') << "')" << std::endl;
}

DISASSEMBLE_INSTRUCTION(EQUALS_INT8) {
    std::cout << "EQUALS_INT8" << std::endl;
}

DISASSEMBLE_INSTRUCTION(EQUALS_INT16) {
    std::cout << "EQUALS_INT16" << std::endl;
}

DISASSEMBLE_INSTRUCTION(EQUALS_INT32) {
    std::cout << "EQUALS_INT32" << std::endl;
}

DISASSEMBLE_INSTRUCTION(EXIT) {
    int8_t exit_code = read(pc);
    std::cout << "EXIT " << (int)exit_code << std::endl;
}


}

