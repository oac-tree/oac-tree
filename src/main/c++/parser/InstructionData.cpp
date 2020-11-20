#include "InstructionData.h"

InstructionData::InstructionData(std::string instr_type)
  : _type{std::move(instr_type)}
{}
