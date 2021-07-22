#include "Lexer.h"
#include <iostream>

bool is_whitespace(char ch) {
    switch(ch) {
    case ' ':
    case '\t':
    case '\r':
    case '\f':
    case '\v':
        return true;
    }
    return false;
}

bool is_valid_char(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

bool is_digit(char ch) {
    return (ch >= '0' && ch <= '9');
}


namespace TIBASIC::Compiler {

Token Lexer::get_next_token() {
    while(m_char != '\0') {

        if(is_whitespace(m_char))
            skip_whitespace();

        if(is_digit(m_char))
            return get_number_token();

        if(is_valid_char(m_char))
            return get_identifier_token();

        if (m_char == '"')
            return get_string_token();

        switch(m_char) {
        case '(':
            advance();
            return create(TokenType::LEFT_PAREN, "(");
        case ')':
            advance();
            return create(TokenType::RIGHT_PAREN, ")");
        case '{':
            advance();
            return create(TokenType::LEFT_BRACKET, "{");
        case '}':
            advance();
            return create(TokenType::RIGHT_BRACKET, "}");
        case '[':
            advance();
            return create(TokenType::LEFT_SQUARE_BRACKET, "[");
        case ']':
            advance();
            return create(TokenType::RIGHT_SQUARE_BRACKET, "]");
        case ',':
            advance();
            return create(TokenType::COMMA, ",");
        case '.':
            advance();
            return create(TokenType::DOT, ".");
        case '+':
            advance();
            return create(TokenType::PLUS, "+");
        case '-':
            if (peek(1) == '>') {
                advance();
                advance();
                return create(TokenType::STORE, "->");
            } else {
                advance();
                return create(TokenType::MINUS, "-");
            }
        case '*':
            advance();
            return create(TokenType::ASTERISK, "*");
        case '/':
            advance();
            return create(TokenType::SLASH, "/");
        case '=':
            advance();
            return create(TokenType::EQUALS, "=");
        case '>':
            if (peek(1) == '=') {
                advance();
                advance();
                return create(TokenType::GREATER_THAN_OR_EQUAL, ">=");
            } else {
                advance();
                return create(TokenType::GREATER_THAN, ">");
            }
        case '<':
            if (peek(1) == '=') {
                advance();
                advance();
                return create(TokenType::LESS_THAN_OR_EQUAL, "<=");
            } else {
                advance();
                return create(TokenType::LESS_THAN, "<");
            }
        case '\n':
            advance();
            return create(TokenType::NEWLINE, "\\n");

        default:
            m_had_error = true;
            std::cout << "Error: Unexpected '" << m_char << "'\n";
            std::cout << "ASCII Value: " << (int)m_char << "\n";
            std::cout << "Position: " << m_pos.line << ":" << m_pos.column << "\n";
            advance();
            break;

        }
    }
    return create(TokenType::END_OF_FILE, "END_OF_FILE");
}

Token Lexer::get_string_token() {
    std::string value;
    advance();
    while(m_char != '"' && m_char != '\n' && m_char != '\0') {
        value.push_back(m_char);
        advance();
    }

    advance();

    return create(TokenType::STRING, value);
}

Token Lexer::get_number_token() {
    std::string value;
    TokenType type = TokenType::INT;

    while(is_digit(m_char)) {
        value.push_back(m_char);
        advance();
    }

    if(m_char == '.') {
        type = TokenType::FLOAT;
        value.push_back(m_char);
        advance();
        while(is_digit(m_char)) {
            value.push_back(m_char);
            advance();
        }
    }

    return create(type, value);

}

Token Lexer::get_identifier_token() {
    std::string value;

    while(is_valid_char(m_char)) {
        value.push_back(m_char);
        advance();
    }

    TokenType type = TokenType::IDENTIFIER;

    type = get_special_identifier(value);

    return create(type, value);
}

void Lexer::skip_line_comment() {}
void Lexer::skip_multiline_comment() {}
void Lexer::skip_whitespace() {
    advance();

    while(is_whitespace(m_char)) {
        advance();
    }
}

void Lexer::advance() {
    m_index++;
    if(m_char == '\n') {
        m_pos.line++;
        m_pos.column = 1;
    } else {
        m_pos.column++;
    }
    m_char = m_source[m_index];
}

char& Lexer::peek(int offset) {
    /*
     * TODO: Check if out-of-bounds
     */
    return m_source[m_index + offset];
}


Token Lexer::create(TokenType type, const std::string& value) {
    return Token(type, value, m_pos.line);
}



std::vector<Token> Lexer::get_tokens() {
    std::vector<Token> tokens;

    while(true) {
        Token token = get_next_token();

        tokens.push_back(token);

        if(token.m_type == TokenType::END_OF_FILE) {
            break;
        }
    }

    return tokens;
}

std::string Lexer::get_source() const {
    return m_source;
}

}
