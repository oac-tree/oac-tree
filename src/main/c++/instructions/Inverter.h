#ifndef INVERTER_H
#define INVERTER_H

#include "DecoratorInstruction.h"

class Inverter : public DecoratorInstruction
{
private:
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

    ExecutionStatus CalculateStatus() const;

public:
    Inverter();

    ~Inverter() override;
};

#endif // INVERTER_H
