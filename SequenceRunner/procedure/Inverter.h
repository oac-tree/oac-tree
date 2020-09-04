#ifndef INVERTOR_H
#define INVERTOR_H

#include "DecoratorInstruction.h"

class Invertor : public DecoratorInstruction
{
private:
    ExecutionStatus ExecuteSingleImpl(Runner * runner) override;

    ExecutionStatus CalculateStatus() const;

public:
    Invertor();

    ~Invertor() override;
};

#endif // INVERTOR_H
