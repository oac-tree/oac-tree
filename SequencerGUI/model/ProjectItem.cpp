#include "ProjectItem.h"

namespace Sequencer {

namespace Model {

ProjectItem::ProjectItem()
    : CompoundItem(ProjectType)
{
    RegisterTag(TagInfo::UniversalTag(SequenceType, {SequenceType}));
    RegisterTag(TagInfo::UniversalTag(VariableType, {VariableType}));
}

ProjectItem::~ProjectItem() {}

}  // namespace Model

}  // namespace Sequencer
