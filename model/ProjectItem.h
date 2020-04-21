#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "Item.h"

namespace Sequencer {

namespace Model {

/**
 * @brief Root item for a whole project.
 * @details ProjectItem is the root item that will contain the full
 * workspace definition, containing sequences, variables, etc.
 */
class ProjectItem : public Item
{
private:

protected:

public:
    /**
     * @brief Constructor
     */
    ProjectItem();

    /**
     * @brief Pure virtual destructor
     */
    ~ProjectItem() override;

    /**
     * @brief Accessor.
     * @details Provides a name for display purposes.
     * @return The name to display.
     */
    std::string GetDisplayName() const override;
};

}  // namespace Model

}  // namespace Sequencer

#endif // PROJECTITEM_H
