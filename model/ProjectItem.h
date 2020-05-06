#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "CompoundItem.h"

namespace Sequencer {

namespace Model {

/**
 * @brief Root item for a whole project.
 * @details ProjectItem is the root item that will contain the full
 * workspace definition, containing sequences, variables, etc.
 */
class ProjectItem : public CompoundItem
{
public:
    /**
     * @brief Constructor
     */
    ProjectItem();

    /**
     * @brief Pure virtual destructor
     */
    ~ProjectItem() override;
};

}  // namespace Model

}  // namespace Sequencer

#endif // PROJECTITEM_H
