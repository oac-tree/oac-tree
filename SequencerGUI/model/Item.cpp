#include "Item.h"
#include "ItemData.h"
#include "ItemRoles.h"
#include "TaggedItems.h"

#include <memory>

namespace Sequencer {

namespace Model {

class ItemImpl
{
private:
    std::unique_ptr<ItemData> __data;
    std::unique_ptr<TaggedItems> __tagged_items;

public:
    ItemImpl();

    ~ItemImpl();

    bool HasData(int role) const;

    QVariant GetData(int role) const;

    bool SetData(const QVariant & value, int role);

    TaggedItems * GetTaggedItems() const;
};

bool Item::CompatibleChild(Item * /*item*/) const
{
    return true;
}

bool Item::SetDataInternal(const QVariant &value, int role)
{
    return __impl->SetData(value, role);
}

Item::Item(ItemType item_type)
    : __item_type(std::move(item_type))
    , __parent(nullptr)
    , __impl(new ItemImpl())
{
    SetDisplayName(__item_type);
}

Item::~Item()
{
    delete __impl;
}

ItemType Item::GetType() const
{
    return __item_type;
}

bool Item::HasData(int role) const
{
    return __impl->HasData(role);
}

QVariant Item::GetDataVariant(int role) const
{
    return __impl->GetData(role);
}

std::string Item::GetDisplayName() const
{
    return GetData<std::string>(ItemRole::Display);
}

void Item::SetDisplayName(std::string name)
{
    __impl->SetData(QVariant::fromValue(name), ItemRole::Display);
}

Item * Item::GetParent() const
{
    return __parent;
}

void Item::SetParent(Item * parent)
{
    __parent = parent;
}

int Item::GetTotalItemCount() const
{
    return __impl->GetTaggedItems()->TotalItemCount();
}

std::vector<Item *> Item::GetAllItems() const
{
    return __impl->GetTaggedItems()->GetAllItems();
}

std::vector<Item *> Item::GetItems(const std::string & tag_name) const
{
    return __impl->GetTaggedItems()->GetItems(tag_name);
}

bool Item::IsTag(const std::string &tag_name) const
{
    return __impl->GetTaggedItems()->IsTag(tag_name);
}

bool Item::RegisterTag(const TagInfo &tag_info)
{
    return __impl->GetTaggedItems()->RegisterTag(tag_info);
}

Item * Item::GetItem(const std::string & tag_name, int row) const
{
    return __impl->GetTaggedItems()->GetItem(tag_name, row);
}

bool Item::InsertItem(Item * item, const std::string &tag_name, int row)
{
    if (item->GetParent())
    {
        return false;
    }
    if (__impl->GetTaggedItems()->InsertItem(item, tag_name, row))
    {
        item->SetParent(this);
        return true;
    }
    return false;
}

ItemImpl::ItemImpl()
    : __data(new ItemData())
    , __tagged_items(new TaggedItems())
{}

ItemImpl::~ItemImpl() {}

bool ItemImpl::HasData(int role) const
{
    return __data->HasData(role);
}

QVariant ItemImpl::GetData(int role) const
{
    return __data->Data(role);
}

bool ItemImpl::SetData(const QVariant &value, int role)
{
    return __data->SetData(value, role);
}

TaggedItems * ItemImpl::GetTaggedItems() const
{
    return __tagged_items.get();
}

}  // namespace Model

}  // namespace Sequencer

