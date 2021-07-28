#include "Bytecode.h"
#include "Op.h"

#include <iomanip>

namespace TIBASIC {

void Bytecode::print_raw_bytecode() {
    int count = 1;
    std::cout << std::hex;;
    for(size_t i = 0; i < m_size; i++) {
        uint8_t byte = m_bytes[i];

        std::cout << std::setfill('0') << std::setw(2) << ((uint16_t)byte & 0xFF) << " ";
        if (count % 8 == 0) {
            std::cout << "\n";
        }
        count++;
    }
    std::cout << std::dec << "\n";
}

void Bytecode::print_constants() {
    int count = 1;
    for(size_t i = 0; i < m_constant_size; i++) {
        Value value = m_constants[i];

        value.print();
        std::cout << ", ";
        if (count % 8 == 0) {
            std::cout << "\n";
        }
        count++;
    }
    std::cout << std::dec << "\n";
}

Bytecode::~Bytecode() {
    delete[] m_bytes;
}

}
