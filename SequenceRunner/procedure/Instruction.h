#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "ExecutionStatus.h"

class UserInterface;
class Workspace;

/**
 * @brief Abstract interface for all executable instructions
 * @details Instruction is the base class for all executable
 * instructions. These include: sequences, selectors, decorator
 * nodes and leaf instruction nodes
 *
 * @note The design of the execution of these instructions
 * is based on private virtual implementation.
 */
class Instruction
{
private:
    ExecutionStatus __status;

    ExecutionStatus __status_before;

    virtual void Preamble(UserInterface * ui);

    virtual ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) = 0;

    virtual void Postamble(UserInterface * ui);

public:
    /**
     * @brief Constructor
     */
    Instruction();

    /**
     * @brief Virtual destructor
     */
    virtual ~Instruction();

    /**
     * @brief Execution method
     * @details
     * @param
     * @return
     */
    void ExecuteSingle(UserInterface * ui, Workspace * ws);

    /**
     * @brief Get execution status
     */
    ExecutionStatus GetStatus() const;

    /**
     * @brief Reset execution status
     */
    void ResetStatus();
};

#endif // INSTRUCTION_H
