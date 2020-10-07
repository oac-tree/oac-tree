#include "SuccesNode.h"

static const std::string SuccesType = "SuccesNode";

ExecutionStatus SuccesNode::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    (void)ui;
    (void)ws;
    return ExecutionStatus::SUCCESS;
}

SuccesNode::SuccesNode()
    : Instruction(SuccesType)
{}

SuccesNode::~SuccesNode()
{}
