#pragma once

#include "Value.h"

#include "pch.h"

namespace TIBASIC {


struct Bytecode {
    uint8_t* m_bytes;
    size_t m_size;

    Value m_constants[512];
    size_t m_constant_size;

    void print_raw_bytecode();
    void print_constants();
    ~Bytecode();
};

}
