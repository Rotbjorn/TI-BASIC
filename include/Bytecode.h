#pragma once

#include "Value.h"

#include <vector>
#include <cstdint>
#include <cstddef>

namespace TIBASIC {


struct Bytecode {
    uint8_t* m_bytes;
    size_t m_size;

    Value m_constants[512];

    void print_dirty();
    ~Bytecode();
};

}
