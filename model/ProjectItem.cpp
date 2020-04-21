#include "ProjectItem.h"

namespace Sequencer {

namespace Model {

ProjectItem::ProjectItem()
    : Item(ProjectType)
{}

ProjectItem::~ProjectItem() {}

std::string ProjectItem::GetDisplayName() const
{
    return "Project root";
}

}  // namespace Model

}  // namespace Sequencer
