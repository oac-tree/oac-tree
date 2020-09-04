#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "CompoundInstruction.h"

class Sequence : public CompoundInstruction
{
private:
    ExecutionStatus ExecuteSingleImpl(Runner * runner) override;

    ExecutionStatus CalculateCompoundStatus() const;

public:
    Sequence();

    ~Sequence() override;
};

#endif // SEQUENCE_H
