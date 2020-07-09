#ifndef PROPERTYITEM_H
#define PROPERTYITEM_H

#include "Item.h"

namespace Sequencer {

namespace Model {

/**
 * @brief The PropertyItem class
 *
 * @details PropertyItem is the main leaf class of the Item hierarchy.
 * It does not contain any child items and only encapsulates basic data.
 */
class PropertyItem : public Item
{
public:
    PropertyItem();

    ~PropertyItem() override;
};

}  // namespace Model

}  // namespace Sequencer

#endif // PROPERTYITEM_H
