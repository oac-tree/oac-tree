#ifndef ITEMCONTAINER_H
#define ITEMCONTAINER_H

#include "Item.h"
#include "TagInfo.h"

#include <vector>

namespace Sequencer {

namespace Model {

class ItemContainer
{
private:
    TagInfo __tag_info;
    std::vector<Item *> __items;

    bool CanInsertItem(Item * item, int row) const;

public:
    ItemContainer(TagInfo tag_info);
    ~ItemContainer();

    int GetSize() const;

    std::string GetTag() const;

    TagInfo GetTagInfo() const;

    bool InsertItem(Item * item, int row=-1);

    Item * GetItem(int row) const;

    Item * RemoveItem(int row);

    std::vector<Item *> GetAllItems() const;
};

}  // namespace Model

}  // namespace Sequencer
#endif // ITEMCONTAINER_H
