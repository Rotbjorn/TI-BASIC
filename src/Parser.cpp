#include "Parser.h"
#include "Op.h"

#include <iostream>
#include <cstring>

namespace TIBASIC::Compiler {



void Parser::parse_statements() {
    parse_statement();
    while(m_curr_token->m_type == TokenType::NEWLINE) {
        if(consume(TokenType::NEWLINE))
            break;

        parse_statement();
    }
}

void Parser::parse_statement() {

    switch(m_curr_token->m_type) {
    case TokenType::IDENTIFIER:
    case TokenType::INT:
    case TokenType::DOUBLE:
    case TokenType::STRING:
        parse_expression();
        break;
    case TokenType::NEWLINE:
        consume(TokenType::NEWLINE);
        break;
    case FUNC_DISP:{
        consume(TokenType::FUNC_DISP);
        parse_expression();

        //if(m_asm.last_opcode == Opcode::EQUALS_INT32) {
        //    m_asm.write_op(Opcode::PRINT_INT8, m_line);
        //} else if (peek(-1).m_type == TokenType::STRING) {
        //    m_asm.write_op(Opcode::PRINT_STRING, m_line);
        //} else {
        //    m_asm.write(Opcode::PRINT_INT32, m_line);
        //}

        break;
    }
    default:
        std::cout << "Unhandled (parse_statement): " << tokentype_to_string(m_curr_token->m_type) << "\n";
        m_had_error = true;
        break;
    }

}

void Parser::parse_expression() {
    parse_term();

    while (
           m_curr_token->m_type == TokenType::PLUS ||
           m_curr_token->m_type == TokenType::MINUS ||
           m_curr_token->m_type == TokenType::EQUALS
           ) {
        Token* op = m_curr_token;
        consume(op->m_type);
        parse_term();
        switch(op->m_type) {
        case TokenType::PLUS:
            //m_asm.write(Opcode::ADD_INT32, op->m_line);
            break;
        case TokenType::MINUS:
            //m_asm.write(Opcode::SUB_INT32, op->m_line);
            break;
        case TokenType::EQUALS:
            //m_asm.write(Opcode::EQUALS_INT32, op->m_line);
            break;
        default:
            std::cout << "Unhandled (parse_expression)\n";
            m_had_error = true;
            break;

        }
    }

    if(m_curr_token->m_type == TokenType::STORE) {
        parse_store();
    }

}

void Parser::parse_term() {
    parse_factor();
}
void Parser::parse_factor() {
    switch (m_curr_token->m_type) {
    case TokenType::INT:
        parse_int();
        break;
    case TokenType::DOUBLE:
        parse_float();
        break;
    case TokenType::IDENTIFIER:
        parse_variable();
        break;
    case TokenType::STRING:
        parse_string();
        break;
    default:
        std::cout << "Unhandled (parse_factor)\n";
        m_had_error = true;
        break;
    }
}


void Parser::parse_int() {
    int32_t value = std::stoi(m_curr_token->m_value);

    //m_asm.write(Opcode::PUSH_INT32, m_line);
    m_asm.write_int32(value, m_line);
    consume(m_curr_token->m_type);

}
void Parser::parse_float() {

}

void Parser::parse_string() {
    std::string value = m_curr_token->m_value;
    std::cout << "parse_string: " << value << std::endl;

    for(char ch : value) {
        //m_asm.write_op(Opcode::PUSH_INT8, m_line);
        m_asm.write((int8_t) ch);
    }
    //m_asm.write_op(Opcode::PUSH_INT8);
    m_asm.write(value.size());
    consume(TokenType::STRING);
}

void Parser::parse_variable() {
    std::string reg = m_curr_token->m_value;
    consume(TokenType::IDENTIFIER);
    //m_asm.write(Opcode::PUSH_REG, m_line);
    m_asm.write(reg[0] - 'A', m_line);
}

void Parser::parse_store() {
    consume(TokenType::STORE);

    int8_t reg = m_curr_token->m_value[0] - 'A';

    // TODO: Fix this flawed logic. If the last thing written wasn't an opcode, will produce failure.
    // Save latest opcode written?
    Opcode last_op = (Opcode) m_asm.m_bytes.back();
    // If last OP pushed INT8, use STORE_INT8 instead
    //if(last_op == EQUALS_INT8 || last_op == EQUALS_INT16 || last_op == EQUALS_INT32) {
    //    m_asm.write(Opcode::STORE_INT8, m_line);
    //} else {
    //    m_asm.write(Opcode::STORE_INT32, m_line);
    //}

    m_asm.write(reg, m_line);

    consume(TokenType::IDENTIFIER);
}

bool Parser::consume(TokenType type) {
    if(m_curr_token->m_type == type) {
        //std::cout << "Consuming " << tokentype_to_string(type) << "\n";
        m_index++;
        Token* token = &m_tokens[m_index];
        if(token->m_type == TokenType::END_OF_FILE) {
            return true;
        }
        m_curr_token = token;
        m_line = m_curr_token->m_line;
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


TIBASIC::Bytecode* Parser::generate_bytecode() {
    parse();
    m_asm.write(Opcode::EXIT, m_line);
    m_asm.write(0, 1);
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
    m_curr_token = m_tokens;
    m_line = m_curr_token->m_line;
}

Parser::~Parser() {
    delete[] m_tokens;
}

}
