#ifndef TAGGEDITEMS_H
#define TAGGEDITEMS_H

#include "ItemContainer.h"

#include <vector>

namespace Sequencer {

namespace Model {

class TaggedItems
{
private:
    std::vector<ItemContainer> __tagged_containers;
    using iterator = std::vector<ItemContainer>::iterator;
    using const_iterator = std::vector<ItemContainer>::const_iterator;
    iterator Container(const std::string& tag_name);
    const_iterator Container(const std::string& tag_name) const;

    TaggedItems(const TaggedItems&);
    TaggedItems& operator=(const TaggedItems&);

public:
    TaggedItems() = default;
    ~TaggedItems() = default;

    // tag

    bool RegisterTag(const TagInfo & tag_info);

    bool IsTag(const std::string & tag_name) const;

    int ItemCount(const std::string & tag_name) const;

    int TotalItemCount() const;

    // adding and removal

    bool InsertItem(Item * item, const std::string & tag_name, int row=-1);

    Item * RemoveItem(const std::string & tag_name, int row);

    bool CanRemoveItem(const std::string & tag_name, int row) const;

    // item access

    Item * GetItem(const std::string & tag_name, int row) const;

    std::vector<Item*> GetItems(const std::string & tag_name) const;

    std::vector<Item*> GetAllItems() const;

//    TagRow tagRowOfItem(const SessionItem* item) const;

//    const_iterator begin() const;
//    const_iterator end() const;

    bool IsPropertyTag(const std::string & tag_name) const;
};

}  // namespace Model

}  // namespace Sequencer

#endif // TAGGEDITEMS_H
