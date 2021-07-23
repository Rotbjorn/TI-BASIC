#include "Parser.h"
#include "Op.h"

#include <iostream>
#include <cstring>

namespace TIBASIC::Compiler {


using ParseFunction = void (Parser::*)();

struct ParseRule {
    ParseFunction infix;
    ParseFunction prefix;
    Precedence precedence;
};


void Parser::parse_statements() {
    parse_expression();
    while(m_curr_token.m_type == TokenType::NEWLINE) {
        if(consume(TokenType::NEWLINE))
            break;

        parse_expression();
    }
    parse_end();
}

void Parser::parse_expression() {
    parse_precedence(Precedence::ASSIGNMENT);
}

void Parser::parse_precedence(Precedence precedence) {

    ParseFunction prefix_function = get_prefix(m_curr_token.m_type);

    if(prefix_function == nullptr) {
        std::cout << "Expected expression\n";
        return;
    }

    (this->*prefix_function)();

    while (precedence <= get_precedence(m_curr_token.m_type)) {
        ParseFunction infix_function = get_infix(m_curr_token.m_type);
        if(!infix_function) {
            std::cout << "Infix function bad???\n";
            return;
        }
        (this->*infix_function)();
    }

}



void Parser::parse_end() {
    m_asm.write(Opcode::EXIT, m_line);
    m_asm.write(0);
}


void Parser::parse_grouping() {
    consume(TokenType::LEFT_PAREN);
    parse_expression();
    consume(TokenType::RIGHT_PAREN);
}

void Parser::parse_unary() {
    TokenType op = m_curr_token.m_type;
    consume(op);

    parse_precedence(Precedence::UNARY);

    switch(op) {
    case TokenType::MINUS:
        m_asm.write_op(Opcode::NEGATE, m_line);
        break;
    default:
        return;
    }

}

void Parser::parse_binary() {
    TokenType op = m_curr_token.m_type;

    consume(op);
    parse_precedence((Precedence) (get_precedence(op) + 1));

    switch(op) {
    case PLUS: m_asm.write_op(Opcode::ADD, m_line); break;
    case MINUS: m_asm.write_op(Opcode::SUBTRACT, m_line); break;
    case ASTERISK: m_asm.write_op(Opcode::MULTIPLY, m_line); break;
    case SLASH: m_asm.write_op(Opcode::DIVIDE, m_line); break;
    default:
        return;
    }
}

void Parser::parse_int() {
    double value = std::stod(m_curr_token.m_value);

    m_asm.write_constant({value}, m_line);

    consume(TokenType::INT);
}

void Parser::parse_double() {
    double value = std::stod(m_curr_token.m_value);

    m_asm.write_constant({value}, m_line);

    consume(TokenType::DOUBLE);
}

void Parser::parse_string() {
    consume(TokenType::STRING);
}

void Parser::parse_variable() {
    consume(TokenType::IDENTIFIER);
}

void Parser::parse_store() {
    consume(TokenType::STORE);
}

bool Parser::consume(TokenType type) {
    if(m_curr_token.m_type == type) {
        //std::cout << "Consuming " << tokentype_to_string(type) << "\n";
        m_index++;
        Token* token = &m_tokens[m_index];
        if(token->m_type == TokenType::END_OF_FILE) {
            return true;
        }
        m_curr_token = *token;
        m_line = m_curr_token.m_line;
        return false;
    } else {
        m_had_error = true;
        std::cout << "Consume error...\n";
        return true;
    }
}

Token Parser::peek(int offset) {
    return m_tokens[m_index + offset];
}


void Parser::parse() {
    parse_statements();
}


Precedence Parser::get_precedence(TokenType type) {
    switch(type) {
    case STRING:                    return NONE;
    case IDENTIFIER:                return NONE;
    case INT:                       return NONE;
    case DOUBLE:                    return NONE;
    case LEFT_PAREN:                return NONE;
    case RIGHT_PAREN:               return NONE;
    case LEFT_BRACKET:              return NONE;
    case RIGHT_BRACKET:             return NONE;
    case LEFT_SQUARE_BRACKET:       return NONE;
    case RIGHT_SQUARE_BRACKET:      return NONE;
    case COMMA:                     return NONE;
    case DOT:                       return NONE;
    case PLUS:                      return TERM;
    case MINUS:                     return TERM;
    case ASTERISK:                  return FACTOR;
    case SLASH:                     return FACTOR;
    case EQUALS:                    return NONE;
    case GREATER_THAN:              return NONE;
    case LESS_THAN:                 return NONE;
    case GREATER_THAN_OR_EQUAL:     return NONE;
    case LESS_THAN_OR_EQUAL:        return NONE;
    case STORE:                     return NONE;
    case KW_IF:                     return NONE;
    case KW_THEN:                   return NONE;
    case KW_WHILE:                  return NONE;
    case KW_END:                    return NONE;
    case FUNC_DISP:                 return NONE;
    case END_OF_FILE:               return NONE;
    case NEWLINE:                   return NONE;
    case UNKNOWN:                   return NONE;
    }
}

ParseFunction Parser::get_infix(TokenType type) {

    switch(type) {
    case STRING: return nullptr;
    case IDENTIFIER: return nullptr;
    case INT: return nullptr;
    case DOUBLE: return nullptr;
    case LEFT_PAREN: return nullptr;
    case RIGHT_PAREN: return nullptr;
    case LEFT_BRACKET: return nullptr;
    case RIGHT_BRACKET: return nullptr;
    case LEFT_SQUARE_BRACKET: return nullptr;
    case RIGHT_SQUARE_BRACKET: return nullptr;
    case COMMA: return nullptr;
    case DOT: return nullptr;
    case PLUS: return &Parser::parse_binary;
    case MINUS: return &Parser::parse_binary;
    case ASTERISK: return &Parser::parse_binary;
    case SLASH: return &Parser::parse_binary;
    case EQUALS: return nullptr;
    case GREATER_THAN: return nullptr;
    case LESS_THAN: return nullptr;
    case GREATER_THAN_OR_EQUAL: return nullptr;
    case LESS_THAN_OR_EQUAL: return nullptr;
    case STORE: return nullptr;
    case KW_IF: return nullptr;
    case KW_THEN: return nullptr;
    case KW_WHILE: return nullptr;
    case KW_END: return nullptr;
    case FUNC_DISP: return nullptr;
    case END_OF_FILE: return nullptr;
    case NEWLINE: return nullptr;
    case UNKNOWN: return nullptr;
    }

}

ParseFunction Parser::get_prefix(TokenType type) {

    switch(type) {
    case STRING: return &Parser::parse_string;
    case IDENTIFIER: return nullptr;
    case INT: return &Parser::parse_int;
    case DOUBLE: return &Parser::parse_double;
    case LEFT_PAREN: return &Parser::parse_grouping;
    case RIGHT_PAREN: return nullptr;
    case LEFT_BRACKET: return nullptr;
    case RIGHT_BRACKET: return nullptr;
    case LEFT_SQUARE_BRACKET: return nullptr;
    case RIGHT_SQUARE_BRACKET: return nullptr;
    case COMMA: return nullptr;
    case DOT: return nullptr;
    case PLUS: return nullptr;
    case MINUS: return &Parser::parse_unary;
    case ASTERISK: return nullptr;
    case SLASH: return nullptr;
    case EQUALS: return nullptr;
    case GREATER_THAN: return nullptr;
    case LESS_THAN: return nullptr;
    case GREATER_THAN_OR_EQUAL: return nullptr;
    case LESS_THAN_OR_EQUAL: return nullptr;
    case STORE: return nullptr;
    case KW_IF: return nullptr;
    case KW_THEN: return nullptr;
    case KW_WHILE: return nullptr;
    case KW_END: return nullptr;
    case FUNC_DISP: return nullptr;
    case END_OF_FILE: return nullptr;
    case NEWLINE: return nullptr;
    case UNKNOWN: return nullptr;
    }

}



TIBASIC::Bytecode* Parser::generate_bytecode() {
    parse();
    return m_asm.get_bytecode();
}

int* Parser::get_debug_lines() {
    int* lines = new int[m_asm.m_lines.size()];
    std::copy(m_asm.m_lines.begin(), m_asm.m_lines.end(), lines);
    return lines;
}

Parser::Parser(std::vector<Token>& tokens) {
    m_tokens = new Token[tokens.size()];
    std::copy(tokens.begin(), tokens.end(), m_tokens);
    m_curr_token = m_tokens[0];
    m_line = m_curr_token.m_line;
}

Parser::~Parser() {
    delete[] m_tokens;
}

}
