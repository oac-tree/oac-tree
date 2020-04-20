#ifndef PRIMITIVEITEMS_H
#define PRIMITIVEITEMS_H

#include "Item.h"

namespace Sequencer {

namespace Model {

class StringItem : public Item
{
private:
    std::string __data;

public:
    /**
     * @brief Constructor
     * @param data String value to encapsulate.
     */
    StringItem(std::string data = "");

    /**
     * @brief Destructor
     */
    ~StringItem() override;

    /**
     * @brief Accessor
     * @param data String value to store.
     */
    void SetData(std::string data);

    /**
     * @brief Accessor
     * @return The Item's type as a display name.
     */
    std::string GetDisplayName() const override;

    /**
     * @brief Accessor
     * @return The encapsulated string value.
     */
    std::string GetData() const;
};

}  // namespace Model

}  // namespace Sequencer

#endif // PRIMITIVEITEMS_H
