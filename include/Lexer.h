#pragma once

#include "Position.h"
#include "Token.h"
#include <vector>



namespace TIBASIC::Compiler {

class Lexer final {
private:
    std::string m_source;
    Position m_pos;
    size_t m_index { 0 };

    char m_char;

    bool m_had_error { false };


private:
    Token get_next_token();
    Token get_string_token();
    Token get_number_token();
    Token get_identifier_token();

    void skip_line_comment();
    void skip_multiline_comment();
    void skip_whitespace();
    void advance();

    char& peek(int offset);

    Token create(TokenType, const std::string&);

public:
    std::vector<Token> get_tokens();
    std::string get_source() const;

    explicit Lexer(const std::string& source) : m_source(source), m_char(source[0]) {}
};

}
