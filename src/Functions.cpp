#include "Functions.h"

#include <cstring>
#include <iostream>

#define Function(name) void name(TIBASIC::Compiler::Parser& parser)

namespace TIBASIC::Functions {

Function(Disp) {
    parser.consume(TokenType::FUNC_DISP);
    parser.parse_expression();
    parser.m_asm.write_op(Opcode::PRINT);
    parser.consume(TokenType::NEWLINE);
}

Function(Input) {
    parser.consume(TokenType::FUNC_INPUT);
    parser.parse_expression();
    parser.m_asm.write_op(Opcode::PRINT);

    parser.consume(TokenType::COMMA, "Expected ',' in Input");

    parser.m_asm.write_op(Opcode::INPUT);

    std::string register_name = parser.m_curr_token.m_value;

    switch(register_name.length()) {
    case 1: {
        size_t reg_index = register_name[0] - 'A';
        parser.m_asm.write_op(Opcode::STORE_NUMBER);
        parser.m_asm.write(reg_index);
        break;
    }
    default:
        std::cerr << "Expected number variable.\n";
    }

    parser.consume(TokenType::IDENTIFIER);
    parser.consume(TokenType::NEWLINE);

}

}
