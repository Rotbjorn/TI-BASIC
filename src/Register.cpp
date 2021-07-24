#include "Register.h"

#include <iostream>
#include <iomanip>

namespace TIBASIC::Runtime {

void Register::display_registers(const char* name) {
    std::cout << "== " << name << " ==\n";
    std::cout << "Number Register [\n    ";
    for(size_t index = 0; index < NUMBER_REGISTER_SIZE; index++) {

        std::cout << std::left << std::setfill(' ');

        // TODO: Optimize...
        std::string str;
        str.push_back((index + 'A'));
        str.append(": ");
        str.append(std::to_string(numbers[index]));
        if(index - 1 != NUMBER_REGISTER_SIZE) {
            str.append(", ");
        }
        std::cout << std::setw(8) << str;;
        if((index + 1) % 5 == 0) {
            std::cout << "\n    ";
        }
    }
    std::cout << "\n]\n\n";
    std::cout << "String Register [\n";
    for(size_t index = 0; index < STRING_REGISTER_SIZE; index++) {
        String current_string = strings[index];
        std::cout << "    Str" << (char)(index + 1 + '0') << ": \'";
        if(current_string.length != 0)
            std::cout << current_string.chars;
        std::cout << "'\n";
    }
    std::cout << "]\n";
}

}
