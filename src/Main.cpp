
#include "Bytecode.h"
#include "Lexer.h"
#include "Op.h"
#include "Parser.h"
#include "VM.h"
#include "Disassembler.h"

#include <iomanip>
#include <iostream>

#define DEBUG_LEXER 0
#define ASSEMBLER_DEBUG 1

int main(int argc, char **argv) {

    if (argc == 0) {
        std::cout << "tibasic - A TI-BASIC interpreter & compiler.\n\n"

        << "usage: tibasic [options] input\n\n"

        << "Commonly used commands: \n"

        << "  tibasic -c main.tb          Compiles 'main.tb' into bytecode\n"
        << "  tibasic main.ctb            Executes bytecode in 'main.ctb'\n"
        << "  tibasic -db main.ctb        Displays readable bytecode from 'main.ctb'\n"

        << "\n"

        << "Options:\n\n"

        << "    -c, --compile               Compiles source into bytecode\n"
        << "    -db, --debug-bytecode       Pretty-prints bytecode file out to readable instructions\n";
        return 0;
    }

    std::string test1 = "Disp \"Hello, World!\nIF A>=B\nTHEN\nDisp 13.123\nEND";
    std::string test2 = "12 + 3 + 4 - 5";
    std::string test3 = "10 + 3->A\n30+5->B";
    std::string test4 = "10->A";
    std::string test5 = "10->A\n1337->B\n421->R\n69->Y";
    std::string test6 = "32->A\nA->B\n69->A\nA+B->C\nC+1->C";
    std::string test7 = "Disp 3";
    std::string test8 = "Disp \"Hello World\"\nDisp \"How are you today";
    std::string test9 = test6 + "\nDisp C";

    TIBASIC::Compiler::Lexer lexer(test9);

    auto tokens = lexer.get_tokens();

    std::cout << "Input: \n```\n" << lexer.get_source() << "\n```\n\n";

    std::cout << "Debug Output: \n```\n";
#if DEBUG_LEXER == 1
    std::setfill(' ');
    std::cout << std::setw(25) << std::left << "Type" << std::setw(15) <<  "Value"  << "Line\n";
    for (auto& token : tokens) {
        std::cout << std::setw(25) << std::left << tokentype_to_string(token.m_type) <<  std::setw(15) << ("'" + token.m_value + "'") << token.m_line << "\n";
    }
#endif

#if 1
    TIBASIC::Bytecode* bc;

    TIBASIC::Compiler::Parser parser(tokens);

#if ASSEMBLER_DEBUG == 1
    using TIBASIC::Opcode;
    TIBASIC::Compiler::Assembler assembler;

    int index = assembler.add_constant({1337.420});

    assembler.write_op(Opcode::CONSTANT);
    assembler.write(index);

    assembler.write(Opcode::EXIT);
    assembler.write(0);
    bc = assembler.get_bytecode();
#else
    bc = parser.generate_bytecode();
#endif
    TIBASIC::Disassembler::disassemble_bytecode(*bc, "Tester Chunk");

    exit(0);

    std::cout << "\n";
    bc->print_dirty();
    std::cout << "\n";

    if(parser.m_had_error) {
        std::cout << "Parser encountered errors; exiting...\n";
        exit(0);
    }

    TIBASIC::Runtime::VM vm;

    std::cout << "== Program started == \n";
    vm.execute(*bc);
    std::cout << "===================== \n";
    std::cout << "Program finished execution with exit code " << vm.exit_code << "\n";

    vm.display_stack("Test Stack (end)");

    vm.reg.display_registers("Test Register (end)");

    std::cout << "\n```\n";

    delete bc;

    return 0;
#endif
}
