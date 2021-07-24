#include "Disassembler.h"

#include <iostream>
#include <iomanip>

#define DISASSEMBLE_INSTRUCTION(n) static void DIS_##n(Bytecode& bc, uint8_t*& pc)

/* Used when instruction doesn't have operand */
#define NO_OPERAND_DISASSEMBLE_INSTRUCTION(n) static void DIS_##n(Bytecode& bc, uint8_t*& pc) {\
    std::cout << #n << std::endl; \
    }


namespace TIBASIC::Disassembler {


using DisassembleInstruction = void (*)(Bytecode&, uint8_t*&);
extern DisassembleInstruction disassemble_instructions[127];


#define __OPCODES(op) \
    static void DIS_##op(Bytecode&, uint8_t*&);

ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES



#define __OPCODES(op)\
    DIS_##op,

DisassembleInstruction disassemble_instructions[127] = {
    ENUMERATE_OPCODES(__OPCODES)
};

#undef __OPCODES

void disassemble_bytecode(TIBASIC::Bytecode& bc, const char* name) {
    std::cout << "== " << name << " ==\n";

    uint8_t* pc = bc.m_bytes;


    while(true) {
        size_t index = pc - bc.m_bytes;

        std::cout << std::right << std::setfill('0') << std::setw(4) << index << "  ";

        int8_t current_instruction = *pc;
        pc++;
        disassemble_instructions[current_instruction](bc, pc);

        if(current_instruction == Opcode::EXIT) break;
    }

}

void disassemble_bytecode(TIBASIC::Bytecode& bc, int* lines, const char* name) {
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
        disassemble_instructions[current_instruction](bc, pc);

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


DISASSEMBLE_INSTRUCTION(CONSTANT) {
    uint8_t index = read(pc);
    std::cout << "CONSTANT " << (int)index << " '";
    bc.m_constants[index].print();
    std::cout << "'" << std::endl;
}

NO_OPERAND_DISASSEMBLE_INSTRUCTION(ADD)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(SUBTRACT)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(MULTIPLY)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(DIVIDE)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(POWER)

NO_OPERAND_DISASSEMBLE_INSTRUCTION(PUSH_TRUE)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(PUSH_FALSE)

NO_OPERAND_DISASSEMBLE_INSTRUCTION(EQUAL)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(NOT_EQUAL)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(GREATER)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(GREATER_OR_EQUAL)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(LESS)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(LESS_OR_EQUAL)

NO_OPERAND_DISASSEMBLE_INSTRUCTION(NEGATE)

NO_OPERAND_DISASSEMBLE_INSTRUCTION(PRINT)
NO_OPERAND_DISASSEMBLE_INSTRUCTION(INPUT)

DISASSEMBLE_INSTRUCTION(STORE_NUMBER) {
    uint8_t reg_index = read(pc);
    std::cout << "STORE_NUMBER " << (char)(reg_index + 'A') << std::endl;
}


DISASSEMBLE_INSTRUCTION(GET_NUMBER) {
    uint8_t reg_index = read(pc);
    std::cout << "GET_NUMBER " << (char)(reg_index + 'A') << std::endl;
}

DISASSEMBLE_INSTRUCTION(STORE_STRING) {
    uint8_t reg_index = read(pc);
    std::cout << "STORE_STRING " << (char)(reg_index + 'A') << std::endl;
}


DISASSEMBLE_INSTRUCTION(GET_STRING) {
    uint8_t reg_index = read(pc);
    std::cout << "GET_STRING " << (char)(reg_index + 'A') << std::endl;
}

NO_OPERAND_DISASSEMBLE_INSTRUCTION(POP)

DISASSEMBLE_INSTRUCTION(JUMP_IF_TRUE) {
    uint16_t offset = read_int16(pc);
    std::cout << "JUMP_IF_TRUE " << offset << std::endl;
}

DISASSEMBLE_INSTRUCTION(JUMP_IF_FALSE) {
    uint16_t offset = read_int16(pc);
    std::cout << "JUMP_IF_FALSE " << offset << std::endl;
}

DISASSEMBLE_INSTRUCTION(JUMP) {
    uint16_t offset = read_int16(pc);
    std::cout << "JUMP " << offset << std::endl;
}

DISASSEMBLE_INSTRUCTION(JUMP_BACK) {
    uint16_t offset = read_int16(pc);
    std::cout << "JUMP_BACK " << offset << std::endl;
}

DISASSEMBLE_INSTRUCTION(EXIT) {
    int8_t exit_code = read(pc);
    std::cout << "EXIT " << (int)exit_code << std::endl;
}


}

