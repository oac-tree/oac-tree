#ifndef COMPOUNDINSTRUCTION_H
#define COMPOUNDINSTRUCTION_H

#include "Instruction.h"

#include <vector>

class CompoundInstruction : public Instruction
{
protected:
    std::vector<Instruction *> __children;

public:
    CompoundInstruction(std::string type);

    ~CompoundInstruction() override;

    void PushBack(Instruction * instruction);
};

#endif // COMPOUNDINSTRUCTION_H
