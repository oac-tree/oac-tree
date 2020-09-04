#ifndef RUNNER_H
#define RUNNER_H

#include "procedure/Procedure.h"
#include "UserInterface.h"

class Instruction;

class Runner
{
private:
    Procedure * __proc;

    UserInterface * __ui;

public:
    Runner(UserInterface * ui);

    ~Runner();

    void SetProcedure(Procedure * procedure);

    void ExecuteSingle();

    void UpdateInstructionStatus(const Instruction * instruction);
};

#endif // RUNNER_H
