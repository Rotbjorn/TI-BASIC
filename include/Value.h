#pragma once

#include <string>

namespace TIBASIC {

enum ValueType {
    STRING,
    BOOLEAN,
    INT,
    DOUBLE
};

struct String {
    char* chars;
    size_t length;

    void concat(String&);

};

struct Value {

    ValueType type;

    union {

        String string;

        bool boolean;
        int integer_number;
        double double_number;
    } as;


    void print();

    bool is_string();
    bool is_boolean();
    bool is_integer();
    bool is_double();

    bool is_number();


    Value();
    Value(String&);
    Value(std::string&);
    Value(bool);
    Value(int);
    Value(double);


    bool operator==(const Value& v);
    bool operator!=(const Value& v);
};


};
