#include "Bytecode.h"
#include "Op.h"

#include <iostream>
#include <iomanip>

namespace TIBASIC {

void Bytecode::print_dirty() {
    int count = 1;
    std::cout << std::hex;;
    for(size_t i = 0; i < m_size; i++) {
        int8_t byte = m_bytes[i];

        std::cout << std::setfill('0') << std::setw(2) << ((int16_t)byte & 0xFF) << " ";
        if (count % 8 == 0) {
            std::cout << "\n";
        }
        count++;
    }
    std::cout << std::dec;
}

}
