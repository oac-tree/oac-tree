#include "DecoratorInstruction.h"


DecoratorInstruction::DecoratorInstruction(std::string type)
    : Instruction(type)
    , __child{}
{}

DecoratorInstruction::~DecoratorInstruction()
{}

void DecoratorInstruction::SetInstruction(Instruction * instruction)
{
    __child.reset(instruction);
}
