#include "VM.h"
#include "Instruction.h"

namespace TIBASIC::Runtime {

void VM::execute(Bytecode& bc){
    pc = bc.m_bytes;
    bytecode = &bc;

    while(true) {
        uint8_t opcode = *pc;

        pc++;

        //std::cout << "Executing opcode: " << opcode_to_string((Opcode) opcode) << "\n";

        instructions[opcode](this);
        if (opcode == Opcode::EXIT) break;
    }
}

void VM::display_stack(const char* name) {
    Value* first = stack.stack;
    int count = 1;

    if (first == stack.stack_pointer) {
        std::cout << "== " << name << " (Empty) ==\n\n";
        return;
    }

    std::cout << "\n== " << name << " ==\n" << std::hex;
    while(first < stack.stack_pointer) {
        std::cout << "[ ";
        first->print();
        std::cout << " ] ";
        if (count % 4 == 0)
            std::cout << "\n";
        first++;
        count++;
    }
    std::cout << std::dec << "\n";
}

}
