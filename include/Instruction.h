#pragma once

#include "Op.h"
#include "VM.h"

namespace TIBASIC {

using Instruction =  void (*)(TIBASIC::Runtime::VM*);
extern Instruction instructions[127];

#define __OPCODES(op) \
    void INS_##op(TIBASIC::Runtime::VM*);

ENUMERATE_OPCODES(__OPCODES)
#undef __OPCODES

}
