#include "SuccessNode.h"

static const std::string SuccessType = "SuccessNode";

ExecutionStatus SuccessNode::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    (void)ui;
    (void)ws;
    return ExecutionStatus::SUCCESS;
}

SuccessNode::SuccessNode()
    : Instruction(SuccessType)
{}

SuccessNode::~SuccessNode()
{}
