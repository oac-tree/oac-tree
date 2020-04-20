#include "SequenceItem.h"

namespace Sequencer {

namespace Model {

SequenceItem::SequenceItem()
    : Item(SequenceType)
{}

SequenceItem::~SequenceItem()
{
    for (auto element : __elements)
    {
        delete element;
    }
}

bool SequenceItem::AddElement(Item *element)
{
    __elements.push_back(element);
    return true;
}

const std::list<Item *> SequenceItem::GetElements() const
{
    return __elements;
}

}  // namespace Model

}  // namespace Sequencer
