#include "DecoratorInstruction.h"


DecoratorInstruction::DecoratorInstruction(std::string type)
    : Instruction(type)
    , _child{}
{}

DecoratorInstruction::~DecoratorInstruction()
{}

void DecoratorInstruction::SetInstruction(Instruction * instruction)
{
    _child.reset(instruction);
}
