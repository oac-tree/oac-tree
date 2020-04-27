#ifndef ITEMDATA_H
#define ITEMDATA_H

#include "UserTypes.h"

#include <utility>
#include <vector>

namespace Sequencer {

namespace Model {

class ItemData
{
private:
    std::vector<std::pair<QVariant, int>> __data;

protected:

public:
    ItemData();
    ~ItemData();

    std::vector<int> Roles() const;

    QVariant Data(int role) const;

    bool SetData(const QVariant & value, int role);

    bool HasData(int role) const;
};

}  // namespace Model

}  // namespace Sequencer

#endif // ITEMDATA_H
