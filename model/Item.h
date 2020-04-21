#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

#include "ItemTypes.h"

namespace Sequencer {

namespace Model {

/**
 * @brief Abstract interface for all model items
 * @details Item is the base class for all model items that
 * need to be represented, created and edited. These include:
 * sequences, primitive values, variable definitions, etc.
 *
 * @note The design is based on the composite pattern to compose
 * tree structures by assembling primitive or composite items into
 * increasingly more complex structures.
 */
class Item
{
private:
    ItemType __item_type;
    std::vector<Item *> __children;

    virtual bool CompatibleChild(Item * item) const;

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
    virtual ~Item();

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

    unsigned GetChildrenCount() const;

    std::vector<Item *> GetChildren() const;

    bool AddChild(Item * item);
};

}  // namespace Model

}  // namespace Sequencer

#endif // ITEM_H
