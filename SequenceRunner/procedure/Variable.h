#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable
{
private:
    std::string __name;
    int __value;

public:
    /**
     * @brief Constructor
     */
    Variable(std::string name);

    /**
     * @brief Destructor
     */
    ~Variable();

    /**
     * @brief Copy constructor
     */
    Variable(const Variable & other);

    /**
     * @brief Get name of variable
     */
    std::string GetName() const;

    /**
     * @brief Get value of variable
     */
    int GetValue() const;

    /**
     * @brief Set value of variable
     */
    bool SetValue(int val);
};

#endif // VARIABLE_H
