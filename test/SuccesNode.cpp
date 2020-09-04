#include "SuccesNode.h"


ExecutionStatus SuccesNode::ExecuteSingleImpl(Runner * runner)
{
    (void)runner;
    return ExecutionStatus::SUCCESS;
}

SuccesNode::SuccesNode()
{}

SuccesNode::~SuccesNode()
{}
