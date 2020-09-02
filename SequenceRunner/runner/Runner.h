#ifndef RUNNER_H
#define RUNNER_H

#include "procedure/ExecutionStatus.h"
#include "procedure/Procedure.h"

class Instruction;

class Runner
{
private:
    Procedure * __proc;

public:
    Runner();

    ~Runner();

    void UpdateInstructionStatus(const Instruction * instruction, ExecutionStatus status);
};

#endif // RUNNER_H
