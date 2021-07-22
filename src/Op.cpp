#include "Op.h"

#include <iostream>


namespace TIBASIC {


const char* opcode_to_string(Opcode code) {
    switch (code) {

#define __OPCODES(op) \
    case Opcode::op: \
    return #op;

    ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES

    }

    return NULL;
}

}
