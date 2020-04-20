#ifndef VARIABLEITEM_H
#define VARIABLEITEM_H

#include "Item.h"

#include <memory>

namespace Sequencer {

namespace Model {

/**
 * @brief The VariableItem class
 *
 * @details This class encapsulates a value, represented by
 * an Item and makes this value available through a variable name.
 */
class VariableItem : public Item
{
private:
    std::string __var_name;
    std::unique_ptr<Item> __value;

public:
    /**
     * @brief Constructor
     * @param name Variable name.
     * @param item Variable value as an Item.
     */
    VariableItem(std::string name, Item * item = nullptr);

    /**
     * @brief Destructor
     */
    ~VariableItem() override;

    /**
     * @brief Accessor
     * @return The variable's name.
     */
    const std::string GetVariableName() const;

    /**
     * @brief Accessor
     * @param name The new name of this variable.
     */
    void SetVariableName(std::string name);

    /**
     * @brief Accessor
     * @return The Item representing the variable's value.
     */
    const Item * GetItem() const;

    /**
     * @brief Accessor
     * @param item Variable value to set.
     * @return true on success.
     */
    bool SetItem(Item * item);
};

}  // namespace Model

}  // namespace Sequencer

#endif // VARIABLEITEM_H
