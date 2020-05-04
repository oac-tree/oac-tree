#include "SequenceItem.h"

namespace Sequencer {

namespace Model {

SequenceItem::SequenceItem()
    : Item(SequenceType)
{
    RegisterTag(TagInfo::UniversalTag(InstructionType));
}

SequenceItem::~SequenceItem() {}

}  // namespace Model

}  // namespace Sequencer
