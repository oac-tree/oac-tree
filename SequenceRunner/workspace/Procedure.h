#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Instruction.h"
#include "Workspace.h"

#include <memory>

class Procedure
{
private:
    std::unique_ptr<Instruction> __root_instruction;
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

};

#endif // PROCEDURE_H
