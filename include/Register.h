#pragma once

// TODO: Move String struct out of Value.h
#include "Value.h"

#include <cstdint>

#define NUMBER_REGISTER_SIZE 26
#define STRING_REGISTER_SIZE 5

#define LIST_COUNT 6
#define LIST_CAPACITY 100

namespace TIBASIC::Runtime {

enum RegisterType : int8_t {
    NumberRegister,
    StringRegister,
    ListRegister

};

enum NumberRegister {
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
    double numbers[NUMBER_REGISTER_SIZE] { 0 };
    String strings[STRING_REGISTER_SIZE];
    double lists[LIST_COUNT][LIST_CAPACITY];

    void display_registers(const char*);
};

}
