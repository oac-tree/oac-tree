#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "procedure/Instruction.h"

class UserInterface
{
public:
    UserInterface() = default;

    virtual ~UserInterface() = default;

    virtual void UpdateInstructionStatus(const Instruction * instruction) = 0;

    virtual void StartSingleStep();

    virtual void EndSingleStep();
};

#endif // USERINTERFACE_H
