#include "DecoratorInstruction.h"


DecoratorInstruction::DecoratorInstruction()
    : __child{}
{}

DecoratorInstruction::~DecoratorInstruction()
{}

void DecoratorInstruction::SetInstruction(Instruction * instruction)
{
    __child.reset(instruction);
}
