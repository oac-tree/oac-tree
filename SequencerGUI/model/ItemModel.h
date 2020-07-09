#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include "Item.h"

namespace Sequencer {

namespace Model {

/**
 * @brief Model that manages an item tree.
 *
 * @details ItemModel is the object that is responsible for the interaction
 * with an Item tree: adding, deleting, changing and moving items.
 */
class ItemModel
{
private:
    Item * __root_item;

protected:

public:
    /**
     * @brief Constructor
     */
    ItemModel();

    /**
     * @brief Destructor
     */
    ~ItemModel();



};

}  // namespace Model

}  // namespace Sequencer

#endif // ITEMMODEL_H
