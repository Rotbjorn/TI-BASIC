#pragma once

#include <cstdint>
#include <string>

#define INT_REGISTER_SIZE 26
#define STRING_REGISTER_SIZE 5
#define LIST_REGISTER_SIZE 6
#define LIST_CAPACITY 100

namespace TIBASIC::Runtime {

enum RegisterType : int8_t {
    Int32Register,
    StringRegister,
    ListRegister

};

enum Int32Register {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z
};

enum StringRegister {
    STR_1,
    STR_2,
    STR_3,
    STR_4,
    STR_5,
};

enum ListRegister {
    L1,
    L2,
    L3,
    L4,
    L5,
    L6
};

struct Register {
    int32_t int_registers[INT_REGISTER_SIZE] { 0 };
    std::string string_registers[STRING_REGISTER_SIZE];
    int32_t list_register[LIST_REGISTER_SIZE][LIST_CAPACITY];

    void display_registers(const char*);
};

}
