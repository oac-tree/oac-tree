#ifndef COMPOUNDITEM_H
#define COMPOUNDITEM_H

#include "PropertyItem.h"
#include "UserTypes.h"

namespace Sequencer {

namespace Model {

class CompoundItem : public Item
{
public:
    CompoundItem(ItemType item_type = CompoundType);

    ~CompoundItem() override;

    template <typename T = PropertyItem> T * AddProperty(const std::string & name);

    template <typename V> PropertyItem * AddProperty(const std::string & name, const V & value);

    //! Register string literals as std::string.
    PropertyItem* AddProperty(const std::string & name, const char * value);
};

template <typename T> T * CompoundItem::AddProperty(const std::string & name)
{
    T * property = new T;
    RegisterTag(TagInfo::PropertyTag(name, property->GetType()));
    property->SetDisplayName(name);
    InsertItem(property, name, 0);
    return property;
}

inline PropertyItem * CompoundItem::AddProperty(const std::string & name, const char * value)
{
    return AddProperty(name, std::string(value));
}

template <typename V>
PropertyItem * CompoundItem::AddProperty(const std::string& name, const V& value)
{
    auto property = new PropertyItem;
    RegisterTag(TagInfo::PropertyTag(name, property->GetType()));
    property->SetDisplayName(name);
    property->SetData(value);
    InsertItem(property, name, 0);
    return property;
}

}  // namespace Model

}  // namespace Sequencer

#endif // COMPOUNDITEM_H
