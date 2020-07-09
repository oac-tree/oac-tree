#include "SequenceItem.h"

namespace Sequencer {

namespace Model {

SequenceItem::SequenceItem()
    : CompoundItem(SequenceType)
{
    RegisterTag(TagInfo::UniversalTag(InstructionType, {InstructionType}));
}

SequenceItem::~SequenceItem() {}

}  // namespace Model

}  // namespace Sequencer
