#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Sequence.h"
#include "Workspace.h"

#include <memory>

class Procedure
{
private:
    std::unique_ptr<Sequence> __root_sequence;
    std::unique_ptr<Workspace> __workspace;

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
