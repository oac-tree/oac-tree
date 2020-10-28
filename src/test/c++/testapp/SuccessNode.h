#ifndef SUCCESSNODE_H
#define SUCCESSNODE_H

#include "instructions/Instruction.h"

class SuccessNode : public Instruction
{
private:
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

public:
    SuccessNode();

    ~SuccessNode() override;
};

#endif // SUCCESSNODE_H
