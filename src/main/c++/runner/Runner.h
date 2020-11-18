#ifndef RUNNER_H
#define RUNNER_H

#include "procedure/Procedure.h"
#include "UserInterface.h"

class Instruction;

class Runner
{
private:
    Procedure * _proc;

    UserInterface * _ui;

public:
    Runner(UserInterface * ui);

    ~Runner();

    void SetProcedure(Procedure * procedure);

    void ExecuteProcedure();

    void ExecuteSingle();

    bool IsFinished() const;
};

#endif // RUNNER_H
