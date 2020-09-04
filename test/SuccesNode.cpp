#include "SuccesNode.h"


ExecutionStatus SuccesNode::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    (void)ui;
    (void)ws;
    return ExecutionStatus::SUCCESS;
}

SuccesNode::SuccesNode()
{}

SuccesNode::~SuccesNode()
{}
