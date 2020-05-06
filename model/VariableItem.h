#ifndef VARIABLEITEM_H
#define VARIABLEITEM_H

#include "CompoundItem.h"

namespace Sequencer {

namespace Model {

/**
 * @brief The VariableItem class
 *
 * @details This class encapsulates a value, represented by
 * an Item and makes this value available through a variable name.
 */
class VariableItem : public CompoundItem
{
public:
    /**
     * @brief Constructor
     */
    VariableItem();

    /**
     * @brief Destructor
     */
    ~VariableItem() override;
};

}  // namespace Model

}  // namespace Sequencer

#endif // VARIABLEITEM_H
