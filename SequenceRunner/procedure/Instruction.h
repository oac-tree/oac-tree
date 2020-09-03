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

    ExecutionStatus __status_before;

    virtual void Preamble(Runner * runner);

    virtual ExecutionStatus ExecuteSingleImpl(Runner * runner) = 0;

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
    void ExecuteSingle(Runner * runner);

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
