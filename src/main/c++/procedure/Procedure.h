#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "instructions/Sequence.h"
#include "Workspace.h"

#include <memory>

/**
 * @brief Procedure contains a tree of instructions
 *
 * @detail A Procedure object contains a full procedure
 * and a workspace
 */
class Procedure
{
private:
    std::unique_ptr<Sequence> _root_sequence;
    std::unique_ptr<Workspace> _workspace;

public:
    /**
     * @brief Constructor
     */
    Procedure();

    /**
     * @brief Destructor
     */
    ~Procedure();

    /**
     * @brief Add variable
     */
    bool AddVariable(Variable var);

    /**
     * @brief List all variable names
     */
    std::vector<std::string> VariableNames() const;

    /**
     * @brief Get variable value
     */
    int GetVariableValue(std::string name);

    /**
     * @brief Set variable value
     */
    bool SetVariableValue(std::string name, int value);

    /**
     * @brief Push Instruction at top level
     */
    bool PushInstruction(Instruction * instruction);

    /**
     * @brief Execute single step of procedure
     */
    void ExecuteSingle(UserInterface * ui);

    /**
     * @brief Retrieve status of root sequence
     */
    ExecutionStatus GetStatus() const;
};

#endif // PROCEDURE_H
