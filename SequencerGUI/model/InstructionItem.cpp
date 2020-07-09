#include "InstructionItem.h"

namespace Sequencer {

namespace Model {

InstructionItem::InstructionItem()
    : CompoundItem(InstructionType)
{
    SetDisplayName("CVVF call");
    AddProperty("RPC service", "SUP::InternalCVVF");
    AddProperty("Function name", "CVVF::SquareValue");
    AddProperty("Timeout (s)", 5.0);
}

InstructionItem::~InstructionItem() {}

}  // namespace Model

}  // namespace Sequencer
