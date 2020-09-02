#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "ExecutionStatus.h"

class Runner;

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

    virtual void Preamble(Runner * runner);

    virtual ExecutionStatus ExecuteImpl(Runner * runner) = 0;

    virtual void Postamble(Runner * runner);

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
    void Execute(Runner * runner);

    /**
     * @brief Reset execution status
     */
    void ResetStatus();
};

#endif // INSTRUCTION_H
