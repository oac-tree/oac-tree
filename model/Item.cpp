#include "Item.h"
#include "ItemData.h"
#include "ItemRoles.h"

#include <memory>

namespace Sequencer {

namespace Model {

class ItemImpl
{
private:
    std::unique_ptr<ItemData> __data;

public:
    ItemImpl();

    ~ItemImpl();

    bool HasData(int role) const;

    QVariant GetData(int role) const;

    bool SetData(const QVariant & value, int role);
};

bool Item::CompatibleChild(Item * /*item*/) const
{
    return true;
}

QVariant Item::GetDataInternal(int role) const
{
    return __impl->GetData(role);
}

Item::Item(ItemType item_type)
    : __impl(new ItemImpl())
    , __item_type(std::move(item_type))
{
    __impl->SetData(QVariant::fromValue(item_type), ItemRole::Display);
}

Item::~Item()
{
    for (auto child : __children)
        delete child;
}

ItemType Item::GetType() const
{
    return __item_type;
}

bool Item::HasData(int role) const
{
    return __impl->HasData(role);
}

std::string Item::GetDisplayName() const
{
    return GetData<std::string>(ItemRole::Display);
}

unsigned Item::GetChildrenCount() const
{
    return __children.size();
}

std::vector<Item *> Item::GetChildren() const
{
    return __children;
}

bool Item::AddChild(Item * item)
{
    if (CompatibleChild(item))
    {
        __children.push_back(item);
        return true;
    }
    return false;
}

ItemImpl::ItemImpl()
    : __data(new ItemData())
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

}  // namespace Model

}  // namespace Sequencer

