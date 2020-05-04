#include "InstructionItem.h"

namespace Sequencer {

namespace Model {

InstructionItem::InstructionItem()
{
    RegisterTag(TagInfo::UniversalTag(StringType));
}

InstructionItem::~InstructionItem() {}

}  // namespace Model

}  // namespace Sequencer
