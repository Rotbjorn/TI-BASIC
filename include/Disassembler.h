#pragma once

#include "Bytecode.h"
#include "Op.h"

namespace TIBASIC::Disassembler {

void disassemble_bytecode(TIBASIC::Bytecode, const char*);

void disassemble_bytecode(TIBASIC::Bytecode, int*, const char*);

};
