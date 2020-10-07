#ifndef SUCCESNODE_H
#define SUCCESNODE_H

#include "procedure/Instruction.h"

class SuccesNode : public Instruction
{
private:
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

public:
    SuccesNode();

    ~SuccesNode() override;
};

#endif // SUCCESNODE_H
