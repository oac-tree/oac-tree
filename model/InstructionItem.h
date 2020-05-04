#ifndef INSTRUCTIONITEM_H
#define INSTRUCTIONITEM_H

#include "Item.h"

namespace Sequencer {

namespace Model {

/**
 * @brief Root item for a whole project.
 * @details ProjectItem is the root item that will contain the full
 * workspace definition, containing sequences, variables, etc.
 */
class InstructionItem : public Item
{
private:

protected:

public:
    /**
     * @brief Constructor
     */
    InstructionItem();

    /**
     * @brief Pure virtual destructor
     */
    ~InstructionItem() override;
};

}  // namespace Model

}  // namespace Sequencer

#endif // INSTRUCTIONITEM_H
