#pragma once

#include "Assembler.h"
#include "Token.h"
#include "Bytecode.h"
#include <vector>

namespace TIBASIC::Compiler {

class Parser {

private:
    Token *m_tokens;
    Token *m_curr_token;

    size_t m_line;
    size_t m_index { 0 };

    Assembler m_asm;

private:
    void parse_statements();
    void parse_statement();

    void parse_expression();
    void parse_term();
    void parse_factor();


    /* Types */
    void parse_int();
    void parse_float();
    void parse_string();
    void parse_variable();

    /* Statements */
    void parse_store();

    bool consume(TokenType);
    Token peek(int);

    void parse();

public:

    TIBASIC::Bytecode generate_bytecode();
    int* get_debug_lines();

    bool m_had_error { false };

    explicit Parser(std::vector<Token>& tokens);
    ~Parser();

};

}
