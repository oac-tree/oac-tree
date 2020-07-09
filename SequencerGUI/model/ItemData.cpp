#include "ItemData.h"

#include <algorithm>

namespace Sequencer {

namespace Model {

ItemData::ItemData() {}

ItemData::~ItemData() {}

std::vector<int> ItemData::Roles() const
{
    std::vector<int> result;
    for (const auto& el : __data)
    {
        result.push_back(el.second);
    }
    return result;
}

QVariant ItemData::Data(int role) const
{
    for (const auto & el : __data)
    {
        if (el.second == role)
        {
            return el.first;
        }
    }
    return QVariant();
}

bool ItemData::SetData(const QVariant &value, int role)
{
    if (!Utils::CompatibleVariantTypes(Data(role), value))
    {
        return false;
    }
    for (auto it = __data.begin(); it != __data.end(); ++it)
    {
        if (it->second == role)
        {
            if (it->first.isValid())
            {
                if (Utils::Equals(it->first, value))
                {
                    return false;
                }
                it->first = value;
            }
            else
            {
                __data.erase(it);
            }
            return true;
        }
    }
    __data.emplace_back(value, role);
    return true;
}

bool ItemData::HasData(int role) const
{
    auto has_role = [role](const std::pair<QVariant, int>& x) { return x.second == role; };
    return std::find_if(__data.begin(), __data.end(), has_role) != __data.end();
}

}  // namespace Model

}  // namespace Sequencer
