#include "Functions.h"

namespace TIBASIC::Functions {

void Disp(TIBASIC::Compiler::Parser& parser) {
    parser.consume(TokenType::FUNC_DISP);
    parser.parse_expression();
    parser.m_asm.write_op(Opcode::PRINT);
    parser.consume(TokenType::NEWLINE);
}

}
