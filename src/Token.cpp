#include "Token.h"

const char* tokentype_to_string(TokenType type) {
    switch (type) {

#define __TOKENTYPE(op) \
    case TokenType::op: \
    return #op;

    ENUMERATE_TOKEN_TYPES(__TOKENTYPE)
#undef __TOKENTYPE

    }

    return NULL;
}

TokenType get_special_identifier(const std::string& str) {
    /* Keywords */
    if(str == "If") return TokenType::KW_IF;
    if(str == "Then") return TokenType::KW_THEN;
    if(str == "End") return TokenType::KW_END;
    if(str == "Else") return TokenType::KW_ELSE;
    if(str == "While") return TokenType::KW_WHILE;
    if(str == "and") return TokenType::KW_AND;
    if(str == "or") return TokenType::KW_OR;
    /* Functions */
    if(str == "Disp") return TokenType::FUNC_DISP;
    if(str == "Input") return TokenType::FUNC_INPUT;

    return TokenType::IDENTIFIER;
}
