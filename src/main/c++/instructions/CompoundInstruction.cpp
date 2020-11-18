#include "CompoundInstruction.h"


CompoundInstruction::CompoundInstruction(std::string type)
    : Instruction(type)
{}

CompoundInstruction::~CompoundInstruction()
{
    for (auto instruction : _children)
    {
        delete instruction;
    }
}

void CompoundInstruction::PushBack(Instruction * instruction)
{
    _children.push_back(instruction);
}
