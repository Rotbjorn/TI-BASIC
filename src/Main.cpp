
#include "Bytecode.h"
#include "Lexer.h"
#include "Op.h"
#include "Parser.h"
#include "VM.h"
#include "Disassembler.h"

#include <iomanip>
#include <iostream>

#define DEBUG_LEXER 1
#define ASSEMBLER_DEBUG 0

void repl() {
    using namespace TIBASIC;
    using namespace TIBASIC::Runtime;
    using namespace TIBASIC::Compiler;


    Bytecode* last_executed_bc = nullptr;
    int* debug_lines = nullptr;

    std::vector<Token> last_lexer_result;

    TIBASIC::Runtime::VM vm;
    while(true) {
        std::string backup_input;
        std::string input;
        while(true) {
            std::string buffer;
            std::cout << " > ";
            getline(std::cin, buffer);
            if(buffer == ":run")
                break;
            else if(buffer == ":raw") {
                if(last_executed_bc) {
                    std::cout << "\nRaw bytecode:\n";
                    last_executed_bc->print_raw_bytecode();
                }
            } else if(buffer == ":lexer") {
                if(last_lexer_result.size() != 0) {
                    std::cout << std::setfill(' ') << std::setw(25) << std::left << "Type" << std::setw(15) <<  "Value"  << "Line\n";
                    for (auto& token : last_lexer_result) {
                        std::cout << std::setw(25) << std::left << tokentype_to_string(token.m_type) <<  std::setw(15) << ("'" + token.m_value + "'") << token.m_line << "\n";
                    }
                }
            } else if(buffer == ":format") {
                if(last_executed_bc && debug_lines) {
                    Disassembler::disassemble_bytecode(*last_executed_bc, debug_lines, "REPL");
                }
            } else if(buffer == ":stack") {
                vm.display_stack("Stack");
            } else if(buffer == ":reg") {
                vm.reg.display_registers("Registers");
            } else if(buffer == ":constants") {
                if(last_executed_bc) {
                    std::cout << "\nConstants:\n";
                    last_executed_bc->print_constants();
                }
            } else if(buffer == ":clear") {
                input.clear();
            }else if(buffer == ":reset") {
                vm = VM();
            } else if(buffer == ":current") {
                std::cout << "Current Input:\n```\n";
                std::cout << input;
                std::cout << "```\n";
            } else if(buffer == ":undo") {
                input = backup_input;
            } else if(buffer == ":help") {
                std::cout << ":run          - runs program\n";
                std::cout << ":lexer        - prints lexer output\n";
                std::cout << ":raw          - prints raw bytecode as hex\n";
                std::cout << ":format       - prints disassembled bytecode\n";
                std::cout << ":stack        - prints stack\n";
                std::cout << ":reg          - prints registers\n";
                std::cout << ":constants    - prints constants\n";
                std::cout << ":clear        - clears current code input\n";
                std::cout << ":current      - prints current code input\n";
                std::cout << ":undo         - undoes latest input (only 1 time)\n";
                std::cout << ":reset        - resets VM state (registers, stacks)\n";
                std::cout << ":help         - prints this\n";
            } else {
                if(buffer.size() != 0) {
                    backup_input = input;
                    input += buffer + '\n';
                }
            }
        }

        Lexer lexer(input);
        auto tokens = lexer.get_tokens();

        Parser parser(tokens);
        Bytecode* bc = parser.generate_bytecode();

        if(!parser.m_had_error) {
            vm.execute(*bc);
        } else {
            std::cout << "Parser error!\n";
        }

        if(last_executed_bc) {
            delete last_executed_bc;
        }

        if(debug_lines) {
            delete[] debug_lines;
        }

        last_executed_bc = bc;
        last_lexer_result = tokens;
        debug_lines = parser.get_debug_lines();
    }
}

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

    repl();

    std::string test1 = "Disp \"Hello, World!\nIF A>=B\nTHEN\nDisp 13.123\nEND";
    std::string test2 = "12 + 3 + 4 - 5";
    std::string test3 = "10 + 3->A\n30+5->B";
    std::string test4 = "10->A";
    std::string test5 = "10->A\n1337->B\n421->R\n69->Y";
    std::string test6 = "32->A\nA->B\n69->A\nA+B->C\nC+1->C";
    std::string test7 = "Disp 3";
    std::string test8 = "Disp \"Hello World\"\nDisp \"How are you today";
    std::string test9 = test6 + "\nDisp C";
    std::string test10 = "12->A\nA->B";

    TIBASIC::Compiler::Lexer lexer(test10);

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

    int index_a = assembler.add_constant({420});
    int index_b = assembler.add_constant({69});

    assembler.write_op(Opcode::CONSTANT);
    assembler.write(index_a);

    assembler.write_op(Opcode::CONSTANT);
    assembler.write(index_b);

    assembler.write_op(Opcode::SUBTRACT);

    assembler.write(Opcode::EXIT);
    assembler.write(0);
    bc = assembler.get_bytecode();
#else
    bc = parser.generate_bytecode();
#endif
    TIBASIC::Disassembler::disassemble_bytecode(*bc, "Tester Chunk");


    std::cout << "\n";
    bc->print_raw_bytecode();
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
