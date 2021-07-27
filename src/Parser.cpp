#include "Functions.h"
#include "Op.h"
#include "Parser.h"

#include <cstring>
#include <iostream>

namespace TIBASIC::Compiler {

using ParseFunction = void (Parser::*)();

struct ParseRule {
    ParseFunction infix;
    ParseFunction prefix;
    Precedence precedence;
};

void Parser::parse() {
    parse_declaration();
    while(m_curr_token.m_type != TokenType::END_OF_FILE) {
        parse_declaration();
    }
    parse_end();
}

void Parser::parse_declaration() {
    int index = find(TokenType::NEWLINE) - 2;
    Token* previous_token = peek(-1);
    Token* store_token = peek(index);
    if((store_token != nullptr) // Make sure that the peek() gave us a token within bounds.
            && (!(m_curr_token.m_type >= TokenType::FUNC_DISP && m_curr_token.m_type <= TokenType::FUNC_INPUT)) // Checks for '%Func% 12->A'
            && (previous_token == nullptr || previous_token->m_type == TokenType::NEWLINE) // Makes sure the last token was either a newline or nullptr (index -1)
            && (store_token->m_type == TokenType::STORE)) {

        parse_store();
    }else {
        parse_statement();
    }
}

void Parser::parse_statement() {
    switch(m_curr_token.m_type) {

    /* Keywords */
    case TokenType::KW_IF:
        parse_if_statement();
        break;
    case TokenType::KW_WHILE:
        parse_while_statement();
        break;

    /* Functions */
    case TokenType::FUNC_DISP:
        Functions::Disp(*this);
        break;
    case TokenType::FUNC_INPUT:
        Functions::Input(*this);
        break;
    default:
        parse_expression();
        consume(TokenType::NEWLINE);
        m_asm.write_op(Opcode::POP);
        break;
    }
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
        if(infix_function == nullptr) {
            return;
        }
        (this->*infix_function)();
    }

}



void Parser::parse_end() {
    m_asm.write(Opcode::EXIT);
    m_asm.write(0);
}


void Parser::parse_grouping() {
    consume(TokenType::LEFT_PAREN);
    parse_expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' to close off grouping.");
}

void Parser::parse_unary() {
    TokenType op = m_curr_token.m_type;
    consume(op);

    parse_precedence(Precedence::UNARY);

    switch(op) {
    case TokenType::MINUS:
        m_asm.write_op(Opcode::NEGATE);
        break;
    default:
        return;
    }

}

void Parser::parse_if_statement() {
    consume(TokenType::KW_IF);
    parse_expression();
    consume(TokenType::NEWLINE);

    size_t if_offset = m_asm.write_jump(Opcode::JUMP_IF_FALSE);
    m_asm.write_op(Opcode::POP);

    if(m_curr_token.m_type == TokenType::KW_THEN) {
        /* Multiple statement in If block */

        consume(TokenType::KW_THEN);
        consume(TokenType::NEWLINE);

        while(m_curr_token.m_type != TokenType::KW_END) {
            parse_declaration();
        }

        consume(TokenType::KW_END);
        consume(TokenType::NEWLINE);
    } else {
        /* Only one statement in If block */
        parse_declaration();
    }


    size_t else_offset = m_asm.write_jump(Opcode::JUMP);

    m_asm.fix_jump(if_offset);
    m_asm.write_op(Opcode::POP);

    if(m_curr_token.m_type == TokenType::KW_ELSE) {
        consume(TokenType::KW_ELSE);
        consume(TokenType::NEWLINE);
        parse_declaration();
    }
    m_asm.fix_jump(else_offset);

}

void Parser::parse_while_statement() {
    size_t loop_begin = m_asm.m_bytes.size();

    consume(TokenType::KW_WHILE);
    parse_expression();
    consume(TokenType::NEWLINE);

    size_t exit_jump = m_asm.write_jump(Opcode::JUMP_IF_FALSE);
    m_asm.write_op(Opcode::POP);

    while(m_curr_token.m_type != TokenType::KW_END) {
        parse_declaration();
    }

    m_asm.write_jump_back(loop_begin);

    consume(TokenType::KW_END, "While statement requires 'End'");
    consume(TokenType::NEWLINE, "Expected new line after 'End'");

    m_asm.fix_jump(exit_jump);
    m_asm.write_op(Opcode::POP);

}

void Parser::parse_binary() {
    TokenType op = m_curr_token.m_type;

    consume(op);
    parse_precedence((Precedence) (get_precedence(op) + 1));

    switch(op) {
    case TokenType::PLUS:               m_asm.write_op(Opcode::ADD); break;
    case TokenType::MINUS:              m_asm.write_op(Opcode::SUBTRACT); break;
    case TokenType::ASTERISK:           m_asm.write_op(Opcode::MULTIPLY); break;
    case TokenType::SLASH:              m_asm.write_op(Opcode::DIVIDE); break;
    case TokenType::ARROW:              m_asm.write_op(Opcode::POWER); break;
    case TokenType::EQUAL:              m_asm.write_op(Opcode::EQUAL); break;
    case TokenType::NOT_EQUAL:          m_asm.write_op(Opcode::NOT_EQUAL); break;
    case TokenType::GREATER:            m_asm.write_op(Opcode::GREATER); break;
    case TokenType::LESS:               m_asm.write_op(Opcode::LESS); break;
    case TokenType::GREATER_OR_EQUAL:   m_asm.write_op(Opcode::LESS_OR_EQUAL); break;
    case TokenType::LESS_OR_EQUAL:      m_asm.write_op(Opcode::LESS_OR_EQUAL); break;
    default:
        return;
    }
}

void Parser::parse_and() {
    consume(TokenType::KW_AND);

    size_t end_jump = m_asm.write_jump(Opcode::JUMP_IF_FALSE);
    m_asm.write_op(Opcode::POP);


    parse_precedence(Precedence::AND);

    m_asm.fix_jump(end_jump);
}

void Parser::parse_or() {
    consume(TokenType::KW_OR);

    size_t else_jump = m_asm.write_jump(Opcode::JUMP_IF_TRUE);
    m_asm.write_op(Opcode::POP);

    parse_precedence(Precedence::OR);
    m_asm.fix_jump(else_jump);
}

void Parser::parse_int() {
    double value = std::stod(m_curr_token.m_value);

    m_asm.write_constant(Value(value));

    consume(TokenType::INT);
}

void Parser::parse_double() {
    double value = std::stod(m_curr_token.m_value);

    m_asm.write_constant(Value(value));

    consume(TokenType::DOUBLE);
}

void Parser::parse_string() {
    std::string value = m_curr_token.m_value;

    m_asm.write_constant(Value(value));
    consume(TokenType::STRING);
}

void Parser::parse_variable() {

    switch(m_curr_token.m_value.length()) {
    case 1: {
        parse_number_variable();
        break;
    }
    case 4: {
        parse_string_variable();
        break;
    }
        // TODO: Add list registers here!
    }
}

void Parser::parse_number_variable() {
    std::string register_name = m_curr_token.m_value;

    size_t reg_index = register_name[0] - 'A';
    m_asm.write_op(Opcode::GET_NUMBER);
    m_asm.write(reg_index);

    consume(TokenType::IDENTIFIER);
}

void Parser::parse_string_variable() {
    std::string register_name = m_curr_token.m_value;

    if(std::memcmp(register_name.c_str(), "Str", 3) != 0) {
        std::cerr << "'" << register_name << "' is not a valid variable name\n";
    }
    size_t reg_index = register_name.back() - '1';
    m_asm.write_op(Opcode::GET_STRING);
    m_asm.write(reg_index);

    consume(TokenType::IDENTIFIER);
}

void Parser::parse_store() {
    parse_expression();
    consume(TokenType::STORE);

    std::string register_name = m_curr_token.m_value;

    switch(register_name.length()) {
    case 1: {
        size_t reg_index = register_name[0] - 'A';
        m_asm.write_op(Opcode::STORE_NUMBER);
        m_asm.write(reg_index);
        break;
    }
    case 4: {
        if(std::memcmp(register_name.c_str(), "Str", 3) != 0) {
            std::cerr << "'" << register_name << "' is not a valid variable name\n";
            break;
        }
        size_t reg_index = register_name.back() - '1';
        m_asm.write_op(Opcode::STORE_STRING);
        m_asm.write(reg_index);
        break;
    }
    }

    consume(TokenType::IDENTIFIER, "Expected variable after store.");
    consume(TokenType::NEWLINE);

}

bool Parser::consume(TokenType type, const char* error_msg) {

    if(type == TokenType::NEWLINE && m_curr_token.m_type == TokenType::END_OF_FILE)
        return true;

    if(m_curr_token.m_type == type) {
        m_index++;
        m_curr_token = m_tokens[m_index];
        m_asm.m_line = m_curr_token.m_line;
        return false;
    } else {
        m_had_error = true;
        std::cout << error_msg << std::endl;
        m_index++;
        m_curr_token = m_tokens[m_index];
        m_asm.m_line = m_curr_token.m_line;
        return true;
    }
}

Token* Parser::peek(int offset) {
    size_t index = m_index + offset;
    if(index < 0 || index > m_size)
        return nullptr;

    return &m_tokens[m_index + offset];
}

int Parser::find(TokenType type) {
    TokenType next_type;
    int i = 1;
    while(true) {
        next_type = peek(i)->m_type;

        if(type == next_type)
            return i;

        if(type == TokenType::NEWLINE && (next_type == TokenType::NEWLINE || next_type == TokenType::END_OF_FILE)) {
            return i;
        }

        if(next_type == TokenType::END_OF_FILE) {
            return -1;
        }

        i += 1;
    }
    return -1;
}


Precedence Parser::get_precedence(TokenType type) {
    switch(type) {
    case TokenType::STRING:                     return Precedence::NONE;
    case TokenType::IDENTIFIER:                 return Precedence::NONE;
    case TokenType::INT:                        return Precedence::NONE;
    case TokenType::DOUBLE:                     return Precedence::NONE;
    case TokenType::LEFT_PAREN:                 return Precedence::NONE;
    case TokenType::RIGHT_PAREN:                return Precedence::NONE;
    case TokenType::LEFT_BRACKET:               return Precedence::NONE;
    case TokenType::RIGHT_BRACKET:              return Precedence::NONE;
    case TokenType::LEFT_SQUARE_BRACKET:        return Precedence::NONE;
    case TokenType::RIGHT_SQUARE_BRACKET:       return Precedence::NONE;
    case TokenType::COMMA:                      return Precedence::NONE;
    case TokenType::DOT:                        return Precedence::NONE;
    case TokenType::PLUS:                       return Precedence::TERM;
    case TokenType::MINUS:                      return Precedence::TERM;
    case TokenType::ASTERISK:                   return Precedence::FACTOR;
    case TokenType::SLASH:                      return Precedence::FACTOR;
    case TokenType::ARROW:                      return Precedence::EXPONENT;
    case TokenType::EQUAL:                      return Precedence::EQUALITY;
    case TokenType::NOT_EQUAL:                  return Precedence::EQUALITY;
    case TokenType::GREATER:                    return Precedence::COMPARISON;
    case TokenType::LESS:                       return Precedence::COMPARISON;
    case TokenType::GREATER_OR_EQUAL:           return Precedence::COMPARISON;
    case TokenType::LESS_OR_EQUAL:              return Precedence::COMPARISON;
    case TokenType::STORE:                      return Precedence::NONE;
    case TokenType::KW_IF:                      return Precedence::NONE;
    case TokenType::KW_THEN:                    return Precedence::NONE;
    case TokenType::KW_END:                     return Precedence::NONE;
    case TokenType::KW_ELSE:                    return Precedence::NONE;
    case TokenType::KW_AND:                     return Precedence::AND;
    case TokenType::KW_OR:                      return Precedence::OR;
    case TokenType::FUNC_DISP:                  return Precedence::NONE;
    case TokenType::END_OF_FILE:                return Precedence::NONE;
    case TokenType::NEWLINE:                    return Precedence::NONE;
    case TokenType::UNKNOWN:                    return Precedence::NONE;
    }
    return NONE;
}

ParseFunction Parser::get_infix(TokenType type) {

    switch(type) {
    case TokenType::STRING:                     return nullptr;
    case TokenType::IDENTIFIER:                 return nullptr;
    case TokenType::INT:                        return nullptr;
    case TokenType::DOUBLE:                     return nullptr;
    case TokenType::LEFT_PAREN:                 return nullptr;
    case TokenType::RIGHT_PAREN:                return nullptr;
    case TokenType::LEFT_BRACKET:               return nullptr;
    case TokenType::RIGHT_BRACKET:              return nullptr;
    case TokenType::LEFT_SQUARE_BRACKET:        return nullptr;
    case TokenType::RIGHT_SQUARE_BRACKET:       return nullptr;
    case TokenType::COMMA:                      return nullptr;
    case TokenType::DOT:                        return nullptr;
    case TokenType::PLUS:                       return &Parser::parse_binary;
    case TokenType::MINUS:                      return &Parser::parse_binary;
    case TokenType::ASTERISK:                   return &Parser::parse_binary;
    case TokenType::SLASH:                      return &Parser::parse_binary;
    case TokenType::ARROW:                      return &Parser::parse_binary;
    case TokenType::EQUAL:                      return &Parser::parse_binary;
    case TokenType::NOT_EQUAL:                  return &Parser::parse_binary;
    case TokenType::GREATER:                    return &Parser::parse_binary;
    case TokenType::LESS:                       return &Parser::parse_binary;
    case TokenType::GREATER_OR_EQUAL:           return &Parser::parse_binary;
    case TokenType::LESS_OR_EQUAL:              return &Parser::parse_binary;
    case TokenType::STORE:                      return nullptr;
    case TokenType::KW_IF:                      return nullptr;
    case TokenType::KW_THEN:                    return nullptr;
    case TokenType::KW_END:                     return nullptr;
    case TokenType::KW_ELSE:                    return nullptr;
    case TokenType::KW_AND:                     return &Parser::parse_and;
    case TokenType::KW_OR:                      return &Parser::parse_or;
    case TokenType::FUNC_DISP:                  return nullptr;
    case TokenType::END_OF_FILE:                return nullptr;
    case TokenType::NEWLINE:                    return nullptr;
    case TokenType::UNKNOWN:                    return nullptr;
    }
    return nullptr;
}

ParseFunction Parser::get_prefix(TokenType type) {

    switch(type) {
    case TokenType::STRING:                     return &Parser::parse_string;
    case TokenType::IDENTIFIER:                 return &Parser::parse_variable;
    case TokenType::INT:                        return &Parser::parse_int;
    case TokenType::DOUBLE:                     return &Parser::parse_double;
    case TokenType::LEFT_PAREN:                 return &Parser::parse_grouping;
    case TokenType::RIGHT_PAREN:                return nullptr;
    case TokenType::LEFT_BRACKET:               return nullptr;
    case TokenType::RIGHT_BRACKET:              return nullptr;
    case TokenType::LEFT_SQUARE_BRACKET:        return nullptr;
    case TokenType::RIGHT_SQUARE_BRACKET:       return nullptr;
    case TokenType::COMMA:                      return nullptr;
    case TokenType::DOT:                        return nullptr;
    case TokenType::PLUS:                       return nullptr;
    case TokenType::MINUS:                      return &Parser::parse_unary;
    case TokenType::ASTERISK:                   return nullptr;
    case TokenType::SLASH:                      return nullptr;
    case TokenType::ARROW:                      return nullptr;
    case TokenType::EQUAL:                      return nullptr;
    case TokenType::NOT_EQUAL:                  return nullptr;
    case TokenType::GREATER:                    return nullptr;
    case TokenType::LESS:                       return nullptr;
    case TokenType::GREATER_OR_EQUAL:           return nullptr;
    case TokenType::LESS_OR_EQUAL:              return nullptr;
    case TokenType::STORE:                      return nullptr;
    case TokenType::KW_IF:                      return nullptr;
    case TokenType::KW_THEN:                    return nullptr;
    case TokenType::KW_END:                     return nullptr;
    case TokenType::KW_ELSE:                    return nullptr;
    case TokenType::KW_AND:                     return nullptr;
    case TokenType::KW_OR:                      return nullptr;
    case TokenType::FUNC_DISP:                  return nullptr;
    case TokenType::END_OF_FILE:                return nullptr;
    case TokenType::NEWLINE:                    return nullptr;
    case TokenType::UNKNOWN:                    return nullptr;
    }
    return nullptr;

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
    m_size = tokens.size();
    m_tokens = new Token[m_size];
    std::copy(tokens.begin(), tokens.end(), m_tokens);
    m_curr_token = m_tokens[0];
    m_asm.m_line = m_curr_token.m_line;
}

Parser::~Parser() {
    delete[] m_tokens;
}

}
