#pragma once

#include "Assembler.h"
#include "Token.h"
#include "Bytecode.h"
#include <vector>

namespace TIBASIC::Compiler {

struct Parser;

enum Precedence {
    NONE,
    ASSIGNMENT,
    OR,
    AND,
    EQUALITY,
    COMPARISON,
    TERM,
    FACTOR,
    EXPONENT,
    UNARY,
    CALL,
    PRIMARY
};

using ParseFunction = void (Parser::*)();

struct Parser {


    Token *m_tokens;
    Token m_curr_token;

    size_t m_index { 0 };
    size_t m_size;

    Assembler m_asm;

    void parse();
    void parse_declaration();
    void parse_statement();

    void parse_expression();
    void parse_precedence(Precedence);
    void parse_end();

    void parse_grouping();
    void parse_unary();
    void parse_binary();

    void parse_and();
    void parse_or();

    /* Statements */
    void parse_if_statement();
    void parse_while_statement();

    /* Types */
    void parse_int();
    void parse_double();
    void parse_string();

    void parse_variable();
    void parse_number_variable();
    void parse_string_variable();

    void parse_store();

    bool consume(TokenType, const char* = "[DEFAULT] » Consume Error!");
    Token* peek(int);
    int find(TokenType);

    /* Rules */
    Precedence get_precedence(TokenType);
    ParseFunction get_infix(TokenType);
    ParseFunction get_prefix(TokenType);


    TIBASIC::Bytecode* generate_bytecode();
    int* get_debug_lines();

    bool m_had_error { false };

    explicit Parser(std::vector<Token>& tokens);
    ~Parser();

};

using ParseFunction = void (Parser::*)();

}
