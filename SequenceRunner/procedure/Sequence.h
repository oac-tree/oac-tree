#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Instruction.h"

#include <vector>

class Sequence : public Instruction
{
private:
    std::vector<Instruction *> __children;

    ExecutionStatus ExecuteSingleImpl(Runner * runner) override;

    ExecutionStatus CalculateCompoundStatus() const;

public:
    Sequence();

    ~Sequence() override;

    void PushBack(Instruction * instruction);
};

#endif // SEQUENCE_H
