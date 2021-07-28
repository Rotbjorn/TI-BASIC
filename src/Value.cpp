#include "Value.h"

#include "pch.h"

namespace TIBASIC {

void String::concat(String& other) {

    size_t new_length = length + other.length;

    char* new_string = new char[new_length + 1];

    std::memcpy(new_string, chars, length);
    std::memcpy(new_string + length, other.chars, other.length);

    new_string[new_length] = '\0';

    //delete[] other.chars;
    //delete[] chars;

    chars = new_string;
    length = new_length;

}


void Value::print() {
    switch(type) {
    case STRING:
        std::cout << "\"" << (as.string.chars) << "\"";
        break;
    case BOOLEAN:
        std::cout << (as.boolean ? "true" : "false");
        break;
    case INT:
        std::cout << as.integer_number;
        break;
    case DOUBLE:
        std::cout << as.double_number;
        break;

    }
}

bool Value::is_string() {
    return (type == ValueType::STRING);
}

bool Value::is_boolean() {
    return (type == ValueType::BOOLEAN);
}

bool Value::is_integer() {
    return (type == ValueType::INT);
}

bool Value::is_double() {
    return (type == ValueType::DOUBLE);
}

bool Value::is_number() {
    return (is_double() || is_integer());
}

Value::Value() {}

Value::Value(String& str) {
    type = ValueType::STRING;
    as.string = str;
}

Value::Value(std::string& value) {
    type = ValueType::STRING;

    String str;

    str.length = value.length();
    str.chars = new char[str.length + 1];

    std::copy(value.begin(), value.end(), str.chars);

    str.chars[str.length] = '\0';

    as.string = str;
}

Value::Value(bool value) {
    type = ValueType::BOOLEAN;
    as.boolean = value;
}

Value::Value(int value) {
    type = ValueType::INT;
    as.integer_number = value;
}

Value::Value(double value) {
    type = ValueType::DOUBLE;
    as.double_number = value;
}

bool Value::operator==(const Value& other) {
    if(type != other.type) return false;
    switch(type) {
    case ValueType::STRING:     return std::strcmp(as.string.chars, other.as.string.chars) == 0;
    case ValueType::BOOLEAN:    return as.boolean == other.as.boolean;
    case ValueType::INT:        return as.integer_number == other.as.integer_number;
    case ValueType::DOUBLE:     return as.double_number == other.as.double_number;
    }
    return false;
}

bool Value::operator!=(const Value& other) {
    return !(*this == other);
}

};
