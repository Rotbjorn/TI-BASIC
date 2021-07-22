#include "Register.h"

#include <iostream>
#include <iomanip>

namespace TIBASIC::Runtime {

void Register::display_registers(const char* name) {
    std::cout << "== " << name << " ==\n";
    for(size_t index = 0; index < INT_REGISTER_SIZE; index++) {

        std::cout << std::left << std::setfill(' ');

        // TODO: Optimize...
        std::string str;
        str.push_back((index + 'A'));
        str.append(": ");
        str.append(std::to_string(int_registers[index]));
        str.append(", ");

        std::cout << std::setw(8) << str;;
        if((index + 1) % 7 == 0) {
            std::cout << "\n";
        }
    }
}

}
