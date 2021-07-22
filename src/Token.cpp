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
    if(str == "IF") return TokenType::KW_IF;
    if(str == "THEN") return TokenType::KW_THEN;
    if(str == "END") return TokenType::KW_END;
    if(str == "WHILE") return TokenType::KW_WHILE;

    /* Functions */
    if(str == "Disp") return TokenType::FUNC_DISP;

    return TokenType::IDENTIFIER;
}
