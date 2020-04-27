#ifndef ITEM_H
#define ITEM_H

#include <QVariant>
#include <string>
#include <vector>

#include "ItemRoles.h"
#include "ItemTypes.h"

namespace Sequencer {

namespace Model {

class ItemImpl;

/**
 * @brief Abstract interface for all model items
 * @details Item is the base class for all model items that
 * need to be represented, created and edited. These include:
 * sequences, primitive values, variable definitions, etc.
 *
 * @note The design is based on the composite pattern to compose
 * tree structures by assembling primitive or composite items into
 * increasingly more complex structures.
 */
class Item
{
private:
    ItemImpl *__impl;
    ItemType __item_type;
    std::vector<Item *> __children;

    virtual bool CompatibleChild(Item * item) const;

    QVariant GetDataInternal(int role) const;

protected:

public:
    /**
     * @brief Constructor
     * @param item_type Typename of this Item
     */
    Item(ItemType item_type = UndefinedType);

    /**
     * @brief Virtual destructor
     */
    virtual ~Item();

    /**
     * @brief Accessor
     * @return The typename of this Item
     */
    ItemType GetType() const;

    bool HasData(int role = ItemRole::Data) const;

    template <typename T> T GetData(int role = ItemRole::Data) const;

    template <typename T> bool SetData(const T& value, int role = ItemRole::Data);

    /**
     * @brief Accessor.
     * @details Provides a name for display purposes.
     * @return The name to display.
     */
    virtual std::string GetDisplayName() const;

    /**
     * @brief Accessor.
     * @details Retrieve the number of child items.
     * @return Number of child items.
     */
    unsigned GetChildrenCount() const;

    /**
     * @brief Accessor.
     * @return A vector of all contained child items.
     */
    std::vector<Item *> GetChildren() const;

    /**
     * @brief Add child item.
     * @param item Item to add as a child.
     * @return true on success.
     */
    bool AddChild(Item * item);
};

template<typename T>
T Item::GetData(int role) const
{
    return GetDataInternal(role).value<T>();
}

template<typename T>
bool Item::SetData(const T & value, int role)
{
    return set_data_internal(QVariant::fromValue(value), role);
}

}  // namespace Model

}  // namespace Sequencer

#endif // ITEM_H
