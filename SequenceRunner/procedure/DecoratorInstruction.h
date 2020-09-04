#ifndef DECORATORINSTRUCTION_H
#define DECORATORINSTRUCTION_H

#include "Instruction.h"

#include <memory>

class DecoratorInstruction : public Instruction
{
protected:
    std::unique_ptr<Instruction> __child;

public:
    DecoratorInstruction();

    ~DecoratorInstruction() override;

    void SetInstruction(Instruction * instruction);
};

#endif // DECORATORINSTRUCTION_H
