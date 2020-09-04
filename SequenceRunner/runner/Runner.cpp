#include "Runner.h"


Runner::Runner()
    : __proc{nullptr}
{}

Runner::~Runner()
{}

void Runner::SetProcedure(Procedure * procedure)
{
    __proc = procedure;
}

void Runner::UpdateInstructionStatus(const Instruction * instruction, ExecutionStatus status)
{

}
