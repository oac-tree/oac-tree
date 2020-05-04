#ifndef ITEM_H
#define ITEM_H

#include <QVariant>
#include <string>
#include <vector>

#include "ItemRoles.h"
#include "ItemTypes.h"
#include "TagInfo.h"

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
    ItemType __item_type;

    Item * __parent;

    virtual bool CompatibleChild(Item * item) const;

    bool SetDataInternal(const QVariant & value, int role);

protected:
    ItemImpl *__impl;

public:
    /**
     * @brief Constructor
     * @param item_type Typename of this Item
     */
    Item(ItemType item_type = BaseItemType);

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

    QVariant GetDataVariant(int role = ItemRole::Data) const;

    template <typename T> bool SetData(const T& value, int role = ItemRole::Data);

    /**
     * @brief Accessor.
     * @details Provides a name for display purposes.
     * @return The name to display.
     */
    virtual std::string GetDisplayName() const;

    /**
     * @brief Set the name to display.
     * @param name Name to display.
     */
    void SetDisplayName(std::string name);

    /**
     * @brief Get the parent item.
     * @return The parent item.
     */
    Item * GetParent() const;

    /**
     * @brief Set the parent item.
     * @param Parent item to set.
     */
    void SetParent(Item * parent);

    /**
     * @brief Accessor.
     * @details Retrieve the number of child items.
     * @param tag_name Name of the tag.
     * @return Number of child items with given tag.
     */
    int GetItemCount(const std::string & tag_name) const;

    /**
     * @brief Accessor.
     * @details Retrieve the number of child items.
     * @return Number of child items.
     */
    int GetTotalItemCount() const;

    /**
     * @brief Accessor.
     * @return A vector of all contained items
     */
    std::vector<Item *> GetAllItems() const;

    /**
     * @brief Accessor.
     * @param tag_name Name of the tag.
     * @return A vector of all contained items with given tag.
     */
    std::vector<Item *> GetItems(const std::string & tag_name) const;

    /**
     * @brief Query presence of tag.
     * @param tag_name Name of the tag.
     * @return true if this item contains the specified tag.
     */
    bool IsTag(const std::string & tag_name) const;

    /**
     * @brief Register a tag.
     * @param tag_info Name and multiplicity of the tag.
     * @return true on succes
     */
    bool RegisterTag(const TagInfo & tag_info);

    /**
     * @brief Accessor.
     * @param tag_name Name of the tag.
     * @param row Row index.
     * @return A vector of all contained items with given tag.
     */
    Item * GetItem(const std::string & tag_name, int row) const;

    /**
     * @brief Add item for given tag.
     * @param item Item to add.
     * @param tag_name Name of the tag.
     * @row Row index.
     * @return true on success.
     */
    bool InsertItem(Item * item, const std::string & tag_name, int row=-1);
};

template<typename T>
T Item::GetData(int role) const
{
    return GetDataVariant(role).value<T>();
}

template<typename T>
bool Item::SetData(const T & value, int role)
{
    return SetDataInternal(QVariant::fromValue(value), role);
}

}  // namespace Model

}  // namespace Sequencer

#endif // ITEM_H
