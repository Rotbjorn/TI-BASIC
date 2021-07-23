#pragma once

#include "Assembler.h"
#include "Token.h"
#include "Bytecode.h"
#include <vector>

namespace TIBASIC::Compiler {



enum Precedence {
    NONE,
    ASSIGNMENT,
    OR,
    AND,
    EQUALITY,
    COMPARISON,
    TERM,
    FACTOR,
    UNARY,
    CALL,
    PRIMARY
};


class Parser {

    using ParseFunction = void (Parser::*)();

private:
    Token *m_tokens;
    Token m_curr_token;

    size_t m_line;
    size_t m_index { 0 };

    Assembler m_asm;

private:
    void parse_statements();
    void parse_expression();
    void parse_precedence(Precedence);
    void parse_end();

    void parse_grouping();
    void parse_unary();
    void parse_binary();

    /* Types */
    void parse_int();
    void parse_double();
    void parse_string();
    void parse_variable();

    /* Statements */
    void parse_store();

    bool consume(TokenType);
    Token peek(int);
    void parse();


    /* Rules */
    Precedence get_precedence(TokenType);
    ParseFunction get_infix(TokenType);
    ParseFunction get_prefix(TokenType);

public:

    TIBASIC::Bytecode* generate_bytecode();
    int* get_debug_lines();

    bool m_had_error { false };

    explicit Parser(std::vector<Token>& tokens);
    ~Parser();

};

using ParseFunction = void (Parser::*)();

}
