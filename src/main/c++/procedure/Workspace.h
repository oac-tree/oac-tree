#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "Variable.h"

#include <map>
#include <vector>

class Workspace
{
private:
    std::map<std::string, Variable> _var_map;

public:
    /**
     * @brief Constructor
     */
    Workspace();

    /**
     * @brief Destructor
     */
    ~Workspace();

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

#endif // WORKSPACE_H
