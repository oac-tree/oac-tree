#ifndef TAGINFO_H
#define TAGINFO_H

#include "ItemTypes.h"

#include <vector>

namespace Sequencer {

namespace Model {

class TagInfo
{
private:
    std::string __name;
    int __min;
    int __max;
    std::vector<ItemType> __item_types;
public:
    TagInfo();
    TagInfo(std::string name, int min, int max, std::vector<ItemType> item_types);

    //! Constructs universal tag intended for unlimited amount of various items.
    static TagInfo UniversalTag(std::string name, std::vector<ItemType> item_types = {});

    //! Constructs tag intended for single property.
    static TagInfo PropertyTag(std::string name, ItemType item_type);

    std::string Name() const;

    int GetMin() const;

    int GetMax() const;

    std::vector<ItemType> GetItemTypes() const;

    bool IsValidItem(const ItemType& item_type) const;

    bool IsPropertyTag() const;
};

}  // namespace Model

}  // namespace Sequencer

#endif // TAGINFO_H
