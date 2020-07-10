#ifndef RUNNER_H
#define RUNNER_H

#include "workspace/ExecutionStatus.h"

class Instruction;

class Runner
{
public:
    Runner();

    virtual ~Runner();

    virtual void UpdateInstructionStatus(const Instruction * instruction, ExecutionStatus status) = 0;

};

#endif // RUNNER_H
