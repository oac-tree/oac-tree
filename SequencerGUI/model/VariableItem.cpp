#include "VariableItem.h"

namespace Sequencer {

namespace Model {

VariableItem::VariableItem()
    : CompoundItem(VariableType)
{
    AddProperty("Variable name", "<var>");
    AddProperty("Variable value", "");
}

VariableItem::~VariableItem() {}

}  // namespace Model

}  // namespace Sequencer
