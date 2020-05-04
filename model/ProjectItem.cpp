#include "ProjectItem.h"

namespace Sequencer {

namespace Model {

ProjectItem::ProjectItem()
    : Item(ProjectType)
{
    RegisterTag(TagInfo::UniversalTag(SequenceType));
}

ProjectItem::~ProjectItem() {}

}  // namespace Model

}  // namespace Sequencer
