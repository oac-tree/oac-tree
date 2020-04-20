#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "ItemTypes.h"

namespace Sequencer {

namespace Model {

/**
 * @brief Abstract interface for all model items
 * @details Todo.
 *
 * @note The design is based on the composite pattern to compose
 * tree structures by assembling primitive or composite items into
 * increasingly more complex structures.
 */
class Item
{
private:
    ItemType __item_type;

protected:

public:
    /**
     * @brief Constructor
     * @param item_type Typename of this Item
     */
    Item(ItemType item_type);

    /**
     * @brief Pure virtual destructor
     */
    virtual ~Item() = 0;

    /**
     * @brief Accessor
     * @return The typename of this Item
     */
    ItemType GetType() const;

    /**
     * @brief Accessor.
     * @details Provides a name for display purposes.
     * @return The name to display.
     */
    virtual std::string GetDisplayName() const = 0;
};

}  // namespace Model

}  // namespace Sequencer

#endif // ITEM_H
