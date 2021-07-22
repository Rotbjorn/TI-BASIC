#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace TIBASIC {


struct Bytecode {
    uint8_t* m_bytes;
    size_t m_size;

    void print_dirty();
};

}
