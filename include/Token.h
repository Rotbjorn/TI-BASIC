#pragma once

#include <string>

#define ENUMERATE_TOKEN_TYPES(O) \
    O(STRING) \
    O(IDENTIFIER) \
    O(INT) \
    O(DOUBLE) \
    O(LEFT_PAREN) \
    O(RIGHT_PAREN) \
    O(LEFT_BRACKET) \
    O(RIGHT_BRACKET) \
    O(LEFT_SQUARE_BRACKET) \
    O(RIGHT_SQUARE_BRACKET) \
    O(COMMA)\
    O(DOT) \
    O(PLUS) \
    O(MINUS) \
    O(ASTERISK) \
    O(SLASH) \
    O(ARROW) \
    O(EQUAL) \
    O(NOT_EQUAL) \
    O(GREATER) \
    O(LESS) \
    O(GREATER_OR_EQUAL) \
    O(LESS_OR_EQUAL) \
    O(STORE) \
    O(KW_IF) \
    O(KW_THEN) \
    O(KW_END) \
    O(KW_ELSE) \
    O(KW_WHILE) \
    O(KW_AND) \
    O(KW_OR) \
    O(FUNC_DISP) \
    O(FUNC_INPUT) \
    O(END_OF_FILE)\
    O(NEWLINE) \
    O(UNKNOWN)\

enum TokenType {
#define __TOKENTYPE(op) \
    op,
    ENUMERATE_TOKEN_TYPES(__TOKENTYPE)
#undef __TOKENTYPE
};

struct Token {
    TokenType m_type;
    std::string m_value;
    int m_line { 0 };

    explicit Token(TokenType type, const std::string& value, int line) : m_type(type), m_value(value), m_line(line) {}
    explicit Token() = default;
};

const char* tokentype_to_string(TokenType);
TokenType get_special_identifier(const std::string& str);
