#include "CompoundInstruction.h"


CompoundInstruction::CompoundInstruction()
{}

CompoundInstruction::~CompoundInstruction()
{
    for (auto instruction : __children)
    {
        delete instruction;
    }
}

void CompoundInstruction::PushBack(Instruction * instruction)
{
    __children.push_back(instruction);
}
