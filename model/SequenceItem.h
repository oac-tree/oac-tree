#ifndef SEQUENCEITEM_H
#define SEQUENCEITEM_H

#include "Item.h"

#include <list>

namespace Sequencer {

namespace Model {

/**
 * @brief The SequenceItem class
 *
 * @details This Item contains a list of subitems that
 * represent instructions to execute, variable definitions, etc.
 */
class SequenceItem : public Item
{
private:
    std::list<Item *> __elements;

public:
    /**
     * @brief Constructor
     */
    SequenceItem();

    /**
     * @brief Destructor
     */
    ~SequenceItem() override;

    /**
     * @brief Add an element to the sequence.
     * @param element Element to add.
     * @return true on success.
     */
    bool AddElement(Item * element);

    /**
     * @brief Get all elements as a list.
     * @return List of contained Items.
     */
    const std::list<Item *> GetElements() const;
};

}  // namespace Model

}  // namespace Sequencer

#endif // SEQUENCEITEM_H
